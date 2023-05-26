import os
import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
import json
import boto3
from botocore.exceptions import NoCredentialsError
from datetime import datetime

def get_db_connection():
    secret_name = "gld_database_secrets"
    region_name = "us-west-1"

    session = boto3.session.Session()
    client = session.client(
        service_name='secretsmanager',
        region_name=region_name
    )

    try:
        print("Retrieving secret value...")
        get_secret_value_response = client.get_secret_value(
            SecretId=secret_name
        )
        print("Secret value retrieved successfully.")
    except NoCredentialsError as e:
        print(f"Error encountered while retrieving secret value: {str(e)}")
        raise Exception('Error in getting DB credentials from AWS Secret Manager')

    if 'SecretString' in get_secret_value_response:
        print("SecretString found in the response.")
        secret = get_secret_value_response['SecretString']
    else:
        print("SecretBinary found in the response.")
        secret = base64.b64decode(get_secret_value_response['SecretBinary'])

    print("Loading DB credentials...")
    db_credentials = json.loads(secret)
    username = db_credentials['username']
    password = db_credentials['password']
    host = db_credentials['host']

    # Split the host value into host and port.
    host, port = host.split(':')

    print("DB credentials loaded successfully.")

    print("Starting connection to the database...")
    conn = psycopg2.connect(
        dbname='postgres',
        user=username,
        password=password,
        host=host,
        port=port  # Use the extracted port here.
    )

    conn.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)
    print("Database connection successful.")

    return conn

def version_handler(event, context):
    try:
        conn = get_db_connection()
        cursor = conn.cursor()

        cursor.execute("""
        CREATE TABLE IF NOT EXISTS latest (
            version TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS checks (
            version TEXT NOT NULL,
            branch TEXT NOT NULL,
            timestamp TIMESTAMP NOT NULL
        );
        """)
        conn.commit()

        # Extract the version and branch from the query string parameters
        version = event['queryStringParameters']['v']
        branch = event['queryStringParameters']['b']

        cursor.execute("""
            INSERT INTO checks(version, branch, timestamp)
            VALUES (%s, %s, %s);
        """, (version, branch, datetime.utcnow()))
        conn.commit()

        cursor.execute("""
            SELECT version FROM latest LIMIT 1;
        """)
        result = cursor.fetchone()
        latest_version = result[0] if result else None

        cursor.close()
        conn.close()

        if latest_version and version < latest_version:
            return {
                'statusCode': 200,
                'body': json.dumps({'latest_version': latest_version})
            }
        else:
            return {
                'statusCode': 200,
                'body': json.dumps('ok')
            }
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps(f'Error: {str(e)}')
        }

def update_latest(event, context):
    try:
        print("Starting database connection...")
        conn = get_db_connection()
        cursor = conn.cursor()
        print("Database connection successful.")

        print("Executing table creation statements...")
        cursor.execute("""
        CREATE TABLE IF NOT EXISTS latest (
            version TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS checks (
            version TEXT NOT NULL,
            branch TEXT NOT NULL,
            timestamp TIMESTAMP NOT NULL
        );
        """)
        conn.commit()
        print("Table creation statements executed successfully.")

        print("Loading event body...")
        if isinstance(event["body"], str):
            request = json.loads(event["body"])
        else:
            request = event["body"]
        version = request["version"]
        print(f"Loaded version: {version}")

        print("Executing update statements...")
        cursor.execute("""
            DELETE FROM latest;
            INSERT INTO latest(version)
            VALUES (%s);
        """, (version,))
        conn.commit()
        print("Update statements executed successfully.")

        cursor.close()
        conn.close()
        print("Database connection closed.")

        return {
            'statusCode': 200,
            'body': json.dumps('ok')
        }
    except Exception as e:
        print(f"Error encountered: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps(f'Error: {str(e)}')
        }
