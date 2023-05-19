import psycopg2
from psycopg2 import sql
import os

def handler(event, context):
    # connect to the database
    conn = psycopg2.connect(
        dbname=os.environ['DB_NAME'],
        user=os.environ['DB_USER'],
        password=os.environ['DB_PASS'],
        host=os.environ['DB_HOST'],
        port=os.environ['DB_PORT']
    )
    cur = conn.cursor()

    # write to the database
    cur.execute("INSERT INTO my_table (id, name) VALUES (%s, %s)", (1, "John"))

    # retrieve from the database
    cur.execute("SELECT name FROM my_table WHERE id = %s", (1,))
    result = cur.fetchone()

    cur.close()
    conn.close()

    return {
        'statusCode': 200,
        'body': result[0]  # return the retrieved name
    }
