provider "aws" {
  region = "us-west-1"
}

data "aws_caller_identity" "current" {}

resource "random_string" "username" {
  length  = 10
  special = false
}

resource "random_password" "password" {
  length           = 16
  special          = true
  override_special = "_%#&"
}

resource "aws_security_group" "lambda_sg" {
  name        = "lambda_sg"
  description = "Allow inbound traffic from Lambda function"
  vpc_id      = aws_vpc.gridlabd.id   # Add this line
}

resource "aws_security_group" "db_sg" {
  name        = "db_sg"
  description = "Allow inbound traffic from Lambda function"
  vpc_id      = aws_vpc.gridlabd.id   # Add this line

  ingress {
    from_port   = 5432
    to_port     = 5432
    protocol    = "tcp"
    security_groups = [aws_security_group.lambda_sg.id]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }
}

resource "aws_subnet" "gridlabd_subnet_1" {
  vpc_id     = aws_vpc.gridlabd.id
  cidr_block = "10.0.2.0/24"
  availability_zone = "us-west-1b"
}

resource "aws_subnet" "gridlabd_subnet_2" {
  vpc_id     = aws_vpc.gridlabd.id
  cidr_block = "10.0.3.0/24"
  availability_zone = "us-west-1c"
}

resource "aws_db_subnet_group" "dbsubnet_group" {
  name       = "dbsubnet_group"
  subnet_ids = [aws_subnet.gridlabd_subnet_1.id, aws_subnet.gridlabd_subnet_2.id]

  tags = {
    Name = "dbsubnet_group"
  }
}

resource "aws_db_instance" "rds_instance" {
  identifier = "gridlabd"
  allocated_storage = 20
  storage_type = "gp2"
  engine = "postgres"
  engine_version = "14.4"
  instance_class = "db.t3.micro"
  username = random_string.username.result
  password = random_password.password.result
  skip_final_snapshot = true
  vpc_security_group_ids = [aws_security_group.db_sg.id]
  db_subnet_group_name = aws_db_subnet_group.dbsubnet_group.name
}

data "archive_file" "lambda_zip" {
  type        = "zip"
  source_dir  = "../lambda"
  output_path = "./lambda.zip"
}

data "aws_s3_bucket" "bucket" {
  bucket = "version.gridlabd.us"
}

resource "aws_s3_bucket_object" "object" {
  bucket = data.aws_s3_bucket.bucket.bucket
  key    = "lambda.zip"
  source = data.archive_file.lambda_zip.output_path
  etag   = filemd5(data.archive_file.lambda_zip.output_path)
}


resource "aws_lambda_function" "version_check" {
  function_name = "version_handler"
  s3_bucket     = data.aws_s3_bucket.bucket.id
  s3_key        = aws_s3_bucket_object.object.key
  handler       = "app.version_handler"  # updated the handler
  role          = aws_iam_role.lambda_role.arn
  runtime       = "python3.10"

  vpc_config {
    security_group_ids = [aws_security_group.lambda_sg.id]
    subnet_ids         = [aws_subnet.gridlabd.id]
  }

  depends_on = [aws_iam_role_policy.lambda_exec_role_policy]
}

data "aws_iam_policy_document" "lambda_exec_policy" {
  statement {
    effect = "Allow"

    actions = [
      "logs:CreateLogGroup",
      "logs:CreateLogStream",
      "logs:PutLogEvents",
    ]

    resources = ["arn:aws:logs:*:*:*"]
  }

  statement {
    effect = "Allow"

    actions = [
      "secretsmanager:GetSecretValue",
    ]

    resources = [data.aws_secretsmanager_secret.db_credentials.arn]
  }

  statement {
    effect = "Allow"

    actions = [
      "ec2:CreateNetworkInterface",
      "ec2:DescribeNetworkInterfaces",
      "ec2:DeleteNetworkInterface",
    ]

    resources = ["*"]
  }

  # Statement for RDS access
  statement {
    effect = "Allow"

    actions = [
      "rds:*",
    ]

    resources = ["*"]
  }
}

resource "aws_iam_role_policy" "lambda_exec_role_policy" {
  role   = aws_iam_role.lambda_role.id
  policy = data.aws_iam_policy_document.lambda_exec_policy.json

  depends_on = [aws_iam_role.lambda_role]
}

resource "aws_iam_role" "lambda_role" {
  name = "gridlabd_lambda_role"

  assume_role_policy = jsonencode({
    Version = "2012-10-17",
    Statement = [
      {
        Action = "sts:AssumeRole",
        Effect = "Allow",
        Principal = {
          Service = "lambda.amazonaws.com"
        }
      },
    ]
  })
}


resource "aws_vpc" "gridlabd" {
  cidr_block = "10.0.0.0/16"
}

resource "aws_subnet" "gridlabd" {
  vpc_id     = aws_vpc.gridlabd.id
  cidr_block = "10.0.1.0/24"
}

data "aws_secretsmanager_secret" "db_credentials" {
  name = "gld_database_secrets"
}

resource "aws_secretsmanager_secret_version" "db_credentials" {
  secret_id     = data.aws_secretsmanager_secret.db_credentials.id
  secret_string = jsonencode({
    username = random_string.username.result
    password = random_password.password.result
    host     = aws_db_instance.rds_instance.endpoint
  })
}

resource "aws_apigatewayv2_api" "api" {
  name          = "gridlabd-api"
  protocol_type = "HTTP"
}

resource "aws_lambda_permission" "apigw" {
  statement_id  = "AllowExecutionFromAPIGateway"
  action        = "lambda:InvokeFunction"
  function_name = aws_lambda_function.version_check.function_name
  principal     = "apigateway.amazonaws.com"

  source_arn = "${aws_apigatewayv2_api.api.execution_arn}/*/*"
}

resource "aws_apigatewayv2_integration" "lambda" {
  api_id           = aws_apigatewayv2_api.api.id
  integration_type = "AWS_PROXY"

  integration_uri = aws_lambda_function.version_check.invoke_arn
}

resource "aws_apigatewayv2_route" "route" {
  api_id    = aws_apigatewayv2_api.api.id
  route_key = "ANY /"
  target    = "integrations/${aws_apigatewayv2_integration.lambda.id}"
}

resource "aws_apigatewayv2_stage" "stage" {
  api_id      = aws_apigatewayv2_api.api.id
  name        = "$default"
  auto_deploy = true
}

resource "aws_lambda_function" "update_latest" {
  function_name = "update_latest"
  s3_bucket     = data.aws_s3_bucket.bucket.id
  s3_key        = aws_s3_bucket_object.object.key
  handler       = "app.update_latest"
  role          = aws_iam_role.lambda_role.arn
  runtime       = "python3.10"

  vpc_config {
    security_group_ids = [aws_security_group.lambda_sg.id]
    subnet_ids         = [aws_subnet.gridlabd.id]
  }

  depends_on = [aws_iam_role_policy.lambda_exec_role_policy]
}

resource "aws_apigatewayv2_integration" "update_lambda" {
  api_id           = aws_apigatewayv2_api.api.id
  integration_type = "AWS_PROXY"
  integration_uri  = aws_lambda_function.update_latest.invoke_arn
}

resource "aws_apigatewayv2_route" "update_route" {
  api_id    = aws_apigatewayv2_api.api.id
  route_key = "POST /update"
  target    = "integrations/${aws_apigatewayv2_integration.update_lambda.id}"
}
