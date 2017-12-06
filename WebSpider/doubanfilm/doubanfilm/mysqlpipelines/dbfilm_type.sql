DROP TABLE IF EXISTS dbfilm_type;
CREATE TABLE dbfilm_type (
    id int(11) NOT NULL AUTO_INCREMENT,
    name varchar(255) DEFAULT NULL,
    url varchar(255) DEFAULT NULL,
    img varchar(255) DEFAULT NULL,
    crew varchar(1000) DEFAULT NULL,
    misc varchar(255) DEFAULT NULL,
    rating varchar(255) DEFAULT NULL,
    `comment_num` varchar(255) DEFAULT NULL,
    PRIMARY KEY(`id`)
) ENGINE=InnoDB
