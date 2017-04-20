USE cx;

第一个例子（无参数）
CREATE PROCEDURE productpricing() 
BEGIN 
SELECT avg(prod_price) AS priceaverage FROM products; 
END;

CALL productpricing();

DROP PROCEDURE productpricing;

第二个例子（有参数，使用了in）
CREATE DEFINER=`root`@`localhost` PROCEDURE `productpricing`(
	OUT pl DECIMAL(8,2),
    OUT ph DECIMAL(8,2),
    OUT pa DECIMAL(8,2)
)
BEGIN
	SELECT Min(prod_price) INTO pl FROM products;
    SELECT Max(prod_price) INTO ph FROM products;
    SELECT Avg(prod_price) INTO pa FROM products;
END;

CALL productpricing(@pricelow, @pricehigh, @priceaverage);

SELECT @pricehigh, @pricelow, @priceaverage;

第三个例子（使用参数，同时使用in和out）
CREATE DEFINER=`root`@`localhost` PROCEDURE `ordertotal`(
	IN onumber INT,
    OUT ototal DECIMAL(8,2)
)
BEGIN
	SELECT Sum(item_price*quantity) FROM orderitems
    WHERE order_num = onumber
    INTO ototal;
END

使用：
CALL ordertotal(20005, @total);

SELECT @total;

CREATE DEFINER=`root`@`localhost` PROCEDURE `ordertotal`(
	IN onumber INT,
    IN taxable BOOLEAN,
    OUT ototal DECIMAL(8,2)
)
    COMMENT 'Obtain order total, optionally adding tax'
BEGIN
	-- Declare variable for total
    DECLARE total DECIMAL(8,2);
    -- Declare tax percentage
	DECLARE taxrate INT DEFAULT 6;
    
    -- Get the order total
    SELECT Sum(item_price*quantity) FROM orderitems
    WHERE order_num = onumber INTO total;
    
    -- Is this taxable?
    IF taxable THEN
		SELECT total+(total/100*taxrate) INTO total;
	END IF;
    
    -- And finally, save to out variable
    SELECT total INTO ototal;
END;

CALL ordertotal(20005, 0, @total);
SELECT @total;

CALL ordertotal(20005, 1, @total);
SELECT @total;

查看存储过程
SHOW CREATE PROCEDURE ordertotal;
SHOW PROCEDURE STATUS LIKE 'processorders';

游标
CREATE DEFINER=`root`@`localhost` PROCEDURE `processorders`()
BEGIN
	-- Declare local variables
    DECLARE done BOOLEAN DEFAULT 0;
    DECLARE o INT;
    DECLARE t DECIMAL(8,2);
    
	-- Declare the cursor
	DECLARE ordernumbers CURSOR
    FOR
    SELECT order_num FROM orders;
    
    -- Declare continue handler
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done=1;

	-- Create a table to store the results
    CREATE TABLE IF NOT EXISTS ordertotals
		(order_num INT, total DECIMAL(8,2));
    
	-- Open the cursor
    OPEN ordernumbers;
    -- Loop through all rows
    REPEAT
		-- Get order number
		FETCH ordernumbers INTO o;
        -- Get the total for this order
        CALL ordertotal(o, 1, t);
        -- Insert order the total into ordertotals
        INSERT INTO ordertotals(order_num, total)
			VALUES(o, t);
    -- End of loop
    UNTIL done END REPEAT;
    
    -- Close the cursor
    CLOSE ordernumbers;
END;

CALL processorders;

SELECT * FROM ordertotals;

DROP TRIGGER IF EXISTS t_afterinsert_on_tab1;
CREATE TRIGGER t_afterinsert_on_tab1 
AFTER INSERT ON tab1
FOR EACH ROW
BEGIN
    INSERT INTO tab2(tab2_id) VALUES(new.tab1_id);
END;

DROP TRIGGER t_afterinsert_on_tab1;

事务
回滚
SELECT * FROM ordertotals;
START TRANSACTION;
SET sql_safe_updates=0;
DELETE FROM ordertotals;
SELECT * FROM ordertotals;
ROLLBACK;
SELECT * FROM ordertotals;

保留点
SAVEPOINT delete1;
ROLLBACK TO delete1;

更改默认的提交行为（非事务语句默认自动提交）
SET autocommit=0;

字符集和校对
显示字符集
SHOW CHARACTER SET;
显示校对
SHOW COLLATION;
SHOW VARIABLES LIKE 'character%';
SHOW VARIABLES LIKE 'collation%';
创建表时指定字符集和校对
CREATE TABLE mytable
(
	column1		INT,
    column2 	VARCHAR(10)
) DEFAULT CHARACTER SET hebrew
  COLLATE hebrew_general_ci;
创建表时为某列指定字符集和校对
CREATE TABLE mytable2
(
	column1 	INT,
    column2		VARCHAR(10),
    column3		VARCHAR(10) CHARACTER SET latin1 COLLATE latin1_general_ci
) DEFAULT CHARACTER SET hebrew
  COLLATE hebrew_general_ci;
SELECT时指定校对
SELECT * FROM customers ORDER BY cust_id, cust_name COLLATE latin1_general_cs;

安全管理
USE mysql;
SELECT user FROM user;
创建账号
CREATE USER jesse IDENTIFIED BY 'password';
更新用户名
RENAME USER jesse TO sai;
删除账号
DROP USER sai;
显示账号的权限
SHOW GRANTS FOR jesse;
设置权限
GRANT SELECT ON cx.* TO jesse;
撤销权限
REVOKE SELECT ON crashcourse.* FROM jesse;
更改密码
SET PASSWORD FOR jesse = Password('n3w password');
更改自己账号密码
SET PASSWORD = Password('n3w password');