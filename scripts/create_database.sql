CREATE DATABASE overkeggly;
USE overkeggly;

CREATE TABLE players (
  id INT UNSIGNED NOT NULL AUTO_INCREMENT,
  name VARCHAR(20) CHARACTER SET utf8mb4 NOT NULL,
  password BINARY(64) NOT NULL,
  x INT UNSIGNED NOT NULL DEFAULT 0,
  y INT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (id),
  UNIQUE KEY name (name)
);

CREATE USER 'overkeggly_user'@'localhost' IDENTIFIED BY 'INSERT PASSWORD HERE';
GRANT ALL ON overkeggly.* TO 'overkeggly_user'@'localhost';
