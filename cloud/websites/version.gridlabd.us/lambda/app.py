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
        get_secret_value_response = client.get_secret_value(
            SecretId=secret_name
        )
    except NoCredentialsError as e:
        print(e)
        raise Exception('Error in getting DB credentials from AWS Secret Manager')

    if 'SecretString' in get_secret_value_response:
        secret = get_secret_value_response['SecretString']
    else:
        secret = base64.b64decode(get_secret_value_response['SecretBinary'])

    db_credentials = json.loads(secret)
    username = db_credentials['username']
    password = db_credentials['password']
    host = db_credentials['host']

    conn = psycopg2.connect(
        dbname='gridlabdVersion',
        user=username,
        password=password,
        host=host,
        port='5432'
    )
    conn.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)

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

        request = json.loads(event["body"])
        version = request["version"]

        cursor.execute("""
            DELETE FROM latest;
            INSERT INTO latest(version)
            VALUES (%s);
        """, (version))
        conn.commit()

        cursor.close()
        conn.close()

        return {
            'statusCode': 200,
            'body': json.dumps('ok')
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps(f'Error: {str(e)}')
        }
