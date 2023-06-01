import os
import re
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

def fetch_stats(cursor, group_by):
    cursor.execute(f"""
        SELECT {group_by}, COUNT(*) FROM versionChecks GROUP BY {group_by};
    """)
    result = cursor.fetchall()
    stats = {k: v for k, v in result}
    return stats


def version_handler(event, context):
    try:
        conn = get_db_connection()
        cursor = conn.cursor()

        cursor.execute("""
        CREATE TABLE IF NOT EXISTS latestMaster (
            version TEXT NOT NULL,
            build TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS latestDevelop (
            version TEXT NOT NULL,
            build TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS versionChecks (
            version TEXT NOT NULL,
            build TEXT NOT NULL,
            branch TEXT NOT NULL,
            timestamp TIMESTAMP NOT NULL
        );
        """)
        conn.commit()

        # Extract query parameters
        query_params = event.get('queryStringParameters', {})
        full_version = query_params.get('v')
        branch = query_params.get('b')
        group_by = query_params.get('by')

        # Handle stats return case
        if not full_version and not branch and (not group_by or group_by == 'version'):
            stats = fetch_stats(cursor, group_by or 'version')
            return {
                'statusCode': 200,
                'body': json.dumps({'groupBy': {group_by or 'version': stats}})
            }

        if not full_version or not branch:
            return {
                'statusCode': 400,
                'body': json.dumps('Both version and branch parameters are required')
            }

        # Split the full version into version and build
        version, build = full_version.split("-")

        if len(version.split('.')) != 3:
            return {'statusCode': 400, 'body': json.dumps('Incorrect version provided')}

        if not re.match(r'\d{2}\d{2}\d{2}', build):
            return {'statusCode': 400, 'body': json.dumps('Build date should be yymmdd format')}

        cursor.execute("""
            INSERT INTO versionChecks(version, build, branch, timestamp)
            VALUES (%s, %s, %s, %s);
        """, (version, build, branch, datetime.utcnow()))
        conn.commit()

        # Handling for different branch scenarios
        if branch in ['master', 'develop']:
            cursor.execute(f"""
                SELECT version, build FROM latest{branch.capitalize()} LIMIT 1;
            """)
            result = cursor.fetchone()
            latest_version, latest_build = result if result else (None, None)

            if latest_version:
                version_comparison = list(map(int, version.split('.'))) < list(map(int, latest_version.split('.')))
                build_comparison = int(build) < int(latest_build)
                if version_comparison or (version == latest_version and build_comparison):
                    return {
                        'statusCode': 200,
                        'body': json.dumps({'latest_version': latest_version, 'latest_build': latest_build})
                    }

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


def update_latest(event, context):
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
    developsk = db_credentials['developsk']
    mastersk = db_credentials['mastersk']

    try:
        print("Starting database connection...")
        conn = get_db_connection()
        cursor = conn.cursor()
        print("Database connection successful.")

        print("Executing table creation statements...")
        cursor.execute("""
        CREATE TABLE IF NOT EXISTS latestMaster (
            version TEXT NOT NULL,
            build TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS latestDevelop (
            version TEXT NOT NULL,
            build TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS versionChecks (
            version TEXT NOT NULL,
            branch TEXT NOT NULL,
            build TEXT NOT NULL,
            timestamp TIMESTAMP NOT NULL
        );
        """)
        conn.commit()

        print("Loading event body...")
        if isinstance(event["body"], str):
            request = json.loads(event["body"])
        else:
            request = event["body"]
        version = request["version"]
        build = request["build"]
        branch = request["branch"]
        sk = request["sk"]
        
        if not version:
            return {'statusCode': 400, 'body': json.dumps('No version received')}
        
        if not sk:
            return {'statusCode': 400, 'body': json.dumps('No sk provided')}
        
        if not build:
            return {'statusCode': 400, 'body': json.dumps('Build date missing from branch')}
        
        if not re.match(r'\d{2}\d{2}\d{2}', build):
            return {'statusCode': 400, 'body': json.dumps('Build date should be yymmdd format')}
        
        if not branch or branch not in ['master', 'develop']:
            return {'statusCode': 400, 'body': json.dumps('Incorrect branch provided')}
        
        if branch == 'master' and sk != mastersk:
            return {'statusCode': 403, 'body': json.dumps('Invalid sk for master branch')}
        
        if branch == 'develop' and sk != developsk:
            return {'statusCode': 403, 'body': json.dumps('Invalid sk for develop branch')}
        
        print(f"Loaded version: {version} and build: {build}")

        print("Executing update statements...")
        cursor.execute(f"""
            DELETE FROM latest{branch.capitalize()};
            INSERT INTO latest{branch.capitalize()}(version, build)
            VALUES (%s, %s);
        """, (version, build,))
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