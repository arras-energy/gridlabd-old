#SET GLOBAL validate_password_policy=low;

CREATE USER gridlabd@'%' IDENTIFIED BY 'gridlabd';
GRANT ALTER ON *.* TO gridlabd@'%' WITH GRANT OPTION;
GRANT CREATE ON *.* TO gridlabd@'%' WITH GRANT OPTION;
GRANT DROP ON *.* TO gridlabd@'%' WITH GRANT OPTION;
GRANT SELECT ON *.* TO gridlabd@'%' WITH GRANT OPTION;
GRANT UPDATE ON *.* TO gridlabd@'%' WITH GRANT OPTION;
GRANT INSERT ON *.* TO gridlabd@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;

CREATE USER gridlabd_ro@'%' IDENTIFIED BY 'gridlabd';
GRANT SELECT ON *.* TO gridlabd_ro@'%';
FLUSH PRIVILEGES;