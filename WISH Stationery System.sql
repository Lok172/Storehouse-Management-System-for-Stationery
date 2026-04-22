-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jan 23, 2025 at 05:40 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `storehouse_management_system`
--
DROP DATABASE IF EXISTS `storehouse_management_system`;
CREATE DATABASE IF NOT EXISTS `storehouse_management_system` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE `storehouse_management_system`;

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

DROP TABLE IF EXISTS `admin`;
CREATE TABLE IF NOT EXISTS `admin` (
  `AdminID` varchar(10) NOT NULL,
  `AdminName` varchar(100) NOT NULL,
  `Password` varchar(20) NOT NULL DEFAULT 'abc@123',
  `HandphoneNo` varchar(20) NOT NULL DEFAULT '',
  `Age` int(10) NOT NULL DEFAULT -1,
  `MarriageStatus` varchar(10) NOT NULL DEFAULT '-',
  `ReferralCode` varchar(10) NOT NULL,
  PRIMARY KEY (`AdminID`),
  UNIQUE KEY `ReferralCode` (`ReferralCode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`AdminID`, `AdminName`, `Password`, `HandphoneNo`, `Age`, `MarriageStatus`, `ReferralCode`) VALUES
('A001', 'Ali bin Muhammad', '123', '0128450132', 40, 'S', 'R001'),
('A002', 'Mei Mei', 'meimei123', '0128423042', 40, 'M', 'R256'),
('A003', 'CHARLIE ONG', 'charlie789', '', 35, '-', 'R123'),
('A004', 'DAVID LIM', 'david012', '0112233445', 60, 'S', 'R456'),
('A005', 'EMILY WONG', 'emily345', '0101234567', -1, 'M', 'R789'),
('A006', 'ALICE TAN', 'alice123', '0123456789', 25, 'S', 'R012'),
('A007', 'BENJAMIN LEE', 'benjamin456', '0129876543', 40, 'M', 'R345');

--
-- Triggers `admin`
--
DROP TRIGGER IF EXISTS `before_insert_admin_handphone`;
DELIMITER $$
CREATE TRIGGER `before_insert_admin_handphone` BEFORE INSERT ON `admin` FOR EACH ROW BEGIN
    IF NEW.HandphoneNo IS NOT NULL AND NEW.HandphoneNo <> '' THEN
        IF NOT (NEW.HandphoneNo REGEXP '^[0-9]{10,11}$') THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HandphoneNo must contain only digits and be 10 to 11 digits long';
        END IF;
    END IF;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_update_admin_handphone`;
DELIMITER $$
CREATE TRIGGER `before_update_admin_handphone` BEFORE UPDATE ON `admin` FOR EACH ROW BEGIN
    IF NEW.HandphoneNo IS NOT NULL AND NEW.HandphoneNo <> '' THEN
        IF NOT (NEW.HandphoneNo REGEXP '^[0-9]{10,11}$') THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HandphoneNo must contain only digits and be 10 to 11 digits long';
        END IF;
    END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `product`
--

DROP TABLE IF EXISTS `product`;
CREATE TABLE IF NOT EXISTS `product` (
  `ProductID` varchar(10) NOT NULL,
  `ProductName` varchar(100) NOT NULL,
  `ProductCategory` varchar(100) NOT NULL,
  `ProductBrand` varchar(100) NOT NULL,
  `ProductPrice` decimal(20,2) NOT NULL,
  `ProductStock` int(10) NOT NULL DEFAULT 0,
  `ProductRemark` varchar(500) NOT NULL,
  PRIMARY KEY (`ProductID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `product`
--

INSERT INTO `product` (`ProductID`, `ProductName`, `ProductCategory`, `ProductBrand`, `ProductPrice`, `ProductStock`, `ProductRemark`) VALUES
('P001', 'Red Pen', 'Writing Instruments', 'Stabilo', 2.80, 470, ''),
('P002', 'Blue Pen', 'Writing Instruments', 'Pentel', 3.00, 515, 'Product is in excellent condition.'),
('P003', 'A4 Paper Pack', 'Paper Products', 'Deli', 8.50, 260, 'Fresh stock available, selling fast.'),
('P004', 'Sticky Notes', 'Organizational Supplies', 'Uni-ball', 5.00, 410, 'Sticky notes are in good condition.'),
('P005', 'Highlighter Set', 'Writing Instruments', 'Stabilo', 9.50, 280, 'Highlighters are new and vibrant.');

--
-- Triggers `product`
--
DROP TRIGGER IF EXISTS `after_insert_product`;
DELIMITER $$
CREATE TRIGGER `after_insert_product` AFTER INSERT ON `product` FOR EACH ROW BEGIN
    INSERT INTO storehouse_management_system.product_backup 
    (`ProductID`, `ProductName`, `ProductCategory`, `ProductBrand`, `ProductPrice`, `ProductStock`, `ProductRemark`)
    VALUES 
    (NEW.`ProductID`, NEW.`ProductName`, NEW.`ProductCategory`, NEW.`ProductBrand`, NEW.`ProductPrice`, NEW.`ProductStock`, NEW.`ProductRemark`);
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `after_update_product`;
DELIMITER $$
CREATE TRIGGER `after_update_product` AFTER UPDATE ON `product` FOR EACH ROW BEGIN
    UPDATE storehouse_management_system.product_backup 
    SET 
        `ProductName` = NEW.`ProductName`,
        `ProductCategory` = NEW.`ProductCategory`,
        `ProductBrand` = NEW.`ProductBrand`,
        `ProductPrice` = NEW.`ProductPrice`,
        `ProductStock` = NEW.`ProductStock`,
        `ProductRemark` = NEW.`ProductRemark`
    WHERE `ProductID` = NEW.`ProductID`;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `product_backup`
--

DROP TABLE IF EXISTS `product_backup`;
CREATE TABLE IF NOT EXISTS `product_backup` (
  `ProductID` varchar(10) NOT NULL,
  `ProductName` varchar(100) NOT NULL,
  `ProductCategory` varchar(100) NOT NULL,
  `ProductBrand` varchar(100) NOT NULL,
  `ProductPrice` decimal(20,2) NOT NULL,
  `ProductStock` int(10) NOT NULL DEFAULT 0,
  `ProductRemark` varchar(500) NOT NULL,
  PRIMARY KEY (`ProductID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `product_backup`
--

INSERT INTO `product_backup` (`ProductID`, `ProductName`, `ProductCategory`, `ProductBrand`, `ProductPrice`, `ProductStock`, `ProductRemark`) VALUES
('P001', 'Red Pen', 'Writing Instruments', 'Stabilo', 2.80, 470, ''),
('P002', 'Blue Pen', 'Writing Instruments', 'Pentel', 3.00, 515, 'Product is in excellent condition.'),
('P003', 'A4 Paper Pack', 'Paper Products', 'Deli', 8.50, 260, 'Fresh stock available, selling fast.'),
('P004', 'Sticky Notes', 'Organizational Supplies', 'Uni-ball', 5.00, 410, 'Sticky notes are in good condition.'),
('P005', 'Highlighter Set', 'Writing Instruments', 'Stabilo', 9.50, 280, 'Highlighters are new and vibrant.');

-- --------------------------------------------------------

--
-- Table structure for table `record_dispatch`
--

DROP TABLE IF EXISTS `record_dispatch`;
CREATE TABLE IF NOT EXISTS `record_dispatch` (
  `DispatchID` varchar(10) NOT NULL,
  `DateDispatch` date NOT NULL DEFAULT current_timestamp(),
  `QuantityDispatch` int(10) NOT NULL,
  `TotalSales` decimal(20,2) NOT NULL,
  `StaffID` varchar(10) NOT NULL,
  `ProductID` varchar(10) NOT NULL,
  PRIMARY KEY (`DispatchID`),
  KEY `ProductID` (`ProductID`),
  KEY `StaffID` (`StaffID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `record_dispatch`
--

INSERT INTO `record_dispatch` (`DispatchID`, `DateDispatch`, `QuantityDispatch`, `TotalSales`, `StaffID`, `ProductID`) VALUES
('D00001', '2024-10-02', 105, 315.00, 'S004', 'P002'),
('D00002', '2024-10-10', 130, 1105.00, 'S003', 'P003'),
('D00003', '2024-10-15', 125, 1187.50, 'S002', 'P005'),
('D00004', '2024-10-22', 145, 725.00, 'S001', 'P004'),
('D00005', '2024-10-28', 120, 336.00, 'S005', 'P001'),
('D00006', '2024-11-01', 100, 850.00, 'S002', 'P003'),
('D00007', '2024-11-01', 150, 1275.00, 'S001', 'P003'),
('D00008', '2024-11-02', 125, 375.00, 'S003', 'P002'),
('D00009', '2024-11-02', 125, 375.00, 'S001', 'P002'),
('D00010', '2024-11-05', 130, 1105.00, 'S005', 'P003'),
('D00011', '2024-11-06', 145, 725.00, 'S003', 'P004'),
('D00012', '2024-11-06', 115, 575.00, 'S001', 'P004'),
('D00013', '2024-11-09', 100, 300.00, 'S002', 'P002'),
('D00014', '2024-11-16', 130, 364.00, 'S002', 'P001'),
('D00015', '2024-11-18', 145, 1232.50, 'S003', 'P003'),
('D00016', '2024-11-19', 145, 725.00, 'S002', 'P004'),
('D00017', '2024-11-23', 140, 420.00, 'S001', 'P002'),
('D00018', '2024-11-27', 105, 294.00, 'S004', 'P001'),
('D00019', '2024-11-30', 150, 1425.00, 'S002', 'P005'),
('D00020', '2024-12-03', 115, 322.00, 'S001', 'P001'),
('D00021', '2024-12-03', 115, 977.50, 'S005', 'P003'),
('D00022', '2024-12-07', 145, 725.00, 'S004', 'P004'),
('D00023', '2024-12-07', 105, 294.00, 'S004', 'P001'),
('D00024', '2024-12-10', 100, 500.00, 'S005', 'P004'),
('D00025', '2024-12-16', 120, 1140.00, 'S005', 'P005'),
('D00026', '2024-12-17', 110, 1045.00, 'S004', 'P005'),
('D00027', '2024-12-23', 115, 1092.50, 'S003', 'P005'),
('D00028', '2024-12-26', 105, 997.50, 'S001', 'P005'),
('D00029', '2024-12-28', 130, 390.00, 'S005', 'P002'),
('D00030', '2024-12-28', 150, 420.00, 'S005', 'P001'),
('D00031', '2025-01-01', 150, 420.00, 'S003', 'P001'),
('D00032', '2025-01-03', 200, 1700.00, 'S001', 'P003'),
('D00033', '2025-01-06', 125, 375.00, 'S004', 'P002'),
('D00034', '2025-01-09', 100, 500.00, 'S002', 'P004'),
('D00035', '2025-01-12', 175, 1662.50, 'S005', 'P005');

-- --------------------------------------------------------

--
-- Table structure for table `record_restock`
--

DROP TABLE IF EXISTS `record_restock`;
CREATE TABLE IF NOT EXISTS `record_restock` (
  `RestockID` varchar(10) NOT NULL,
  `DateSupplied` date NOT NULL DEFAULT current_timestamp(),
  `QuantitySupplied` int(10) NOT NULL,
  `ProductID` varchar(10) NOT NULL,
  `SupplierID` varchar(10) NOT NULL,
  `StaffID` varchar(10) NOT NULL,
  PRIMARY KEY (`RestockID`),
  KEY `ProductID` (`ProductID`),
  KEY `SupplierID` (`SupplierID`),
  KEY `StaffID` (`StaffID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `record_restock`
--

INSERT INTO `record_restock` (`RestockID`, `DateSupplied`, `QuantitySupplied`, `ProductID`, `SupplierID`, `StaffID`) VALUES
('B00001', '2024-10-01', 155, 'P003', 'C004', 'S001'),
('B00002', '2024-10-03', 160, 'P001', 'C002', 'S005'),
('B00003', '2024-10-05', 175, 'P005', 'C005', 'S003'),
('B00004', '2024-10-07', 180, 'P002', 'C001', 'S004'),
('B00005', '2024-10-09', 195, 'P004', 'C003', 'S002'),
('B00006', '2024-10-11', 170, 'P002', 'C005', 'S004'),
('B00007', '2024-10-14', 185, 'P001', 'C003', 'S002'),
('B00008', '2024-10-16', 150, 'P005', 'C002', 'S001'),
('B00009', '2024-10-18', 200, 'P004', 'C004', 'S005'),
('B00010', '2024-10-21', 165, 'P003', 'C001', 'S003'),
('B00011', '2024-11-01', 195, 'P004', 'C002', 'S001'),
('B00012', '2024-11-05', 185, 'P002', 'C005', 'S004'),
('B00013', '2024-11-09', 200, 'P003', 'C003', 'S002'),
('B00014', '2024-11-13', 190, 'P001', 'C004', 'S003'),
('B00015', '2024-11-15', 170, 'P005', 'C001', 'S005'),
('B00016', '2024-11-18', 175, 'P003', 'C005', 'S001'),
('B00017', '2024-11-20', 180, 'P002', 'C004', 'S003'),
('B00018', '2024-11-22', 155, 'P001', 'C002', 'S004'),
('B00019', '2024-11-25', 165, 'P004', 'C003', 'S002'),
('B00020', '2024-11-28', 160, 'P005', 'C001', 'S005'),
('B00021', '2024-12-02', 200, 'P001', 'C003', 'S002'),
('B00022', '2024-12-04', 195, 'P002', 'C004', 'S005'),
('B00023', '2024-12-06', 185, 'P003', 'C001', 'S003'),
('B00024', '2024-12-09', 180, 'P004', 'C005', 'S004'),
('B00025', '2024-12-11', 175, 'P005', 'C002', 'S001'),
('B00026', '2024-12-14', 160, 'P001', 'C004', 'S003'),
('B00027', '2024-12-16', 165, 'P002', 'C001', 'S005'),
('B00028', '2024-12-18', 155, 'P003', 'C005', 'S002'),
('B00029', '2024-12-20', 190, 'P004', 'C002', 'S004'),
('B00030', '2024-12-23', 150, 'P005', 'C003', 'S001'),
('B00031', '2025-01-02', 200, 'P005', 'C003', 'S002'),
('B00032', '2025-01-04', 195, 'P003', 'C002', 'S005'),
('B00033', '2025-01-06', 185, 'P002', 'C005', 'S004'),
('B00034', '2025-01-08', 180, 'P004', 'C001', 'S003'),
('B00035', '2025-01-10', 175, 'P001', 'C004', 'S001');

-- --------------------------------------------------------

--
-- Table structure for table `staff`
--

DROP TABLE IF EXISTS `staff`;
CREATE TABLE IF NOT EXISTS `staff` (
  `StaffID` varchar(10) NOT NULL,
  `StaffName` varchar(100) NOT NULL,
  `Password` varchar(20) NOT NULL DEFAULT 'abc@123',
  `HandphoneNo` varchar(20) NOT NULL DEFAULT '',
  `Age` int(10) NOT NULL DEFAULT -1,
  `MarriageStatus` varchar(10) NOT NULL DEFAULT '-',
  `AdminID` varchar(10) NOT NULL,
  PRIMARY KEY (`StaffID`),
  KEY `AdminID` (`AdminID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `staff`
--

INSERT INTO `staff` (`StaffID`, `StaffName`, `Password`, `HandphoneNo`, `Age`, `MarriageStatus`, `AdminID`) VALUES
('S001', 'Muhammad abu', '123', '', -1, '-', 'A002'),
('S002', 'Liu Wei', 'def5678', '0123456789', 30, 'S', 'A001'),
('S003', 'Aisha Kumar', 'ghi9012', '0112233445', 45, 'M', 'A003'),
('S004', 'John Smith', 'jkl3456', '', 50, '-', 'A001'),
('S005', 'Emily Tan', 'mno7890', '0109876543', -1, 'S', 'A003');

--
-- Triggers `staff`
--
DROP TRIGGER IF EXISTS `after_insert_staff`;
DELIMITER $$
CREATE TRIGGER `after_insert_staff` AFTER INSERT ON `staff` FOR EACH ROW BEGIN
    INSERT INTO `storehouse_management_system`.`staff_backup` 
    (`StaffID`, `StaffName`, `Password`, `HandphoneNo`, `Age`, `MarriageStatus`)
    VALUES 
    (NEW.`StaffID`, NEW.`StaffName`, NEW.`Password`, NEW.`HandphoneNo`, NEW.`Age`, NEW.`MarriageStatus`);
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `after_update_staff`;
DELIMITER $$
CREATE TRIGGER `after_update_staff` AFTER UPDATE ON `staff` FOR EACH ROW BEGIN
    UPDATE `storehouse_management_system`.`staff_backup` 
    SET 
        `StaffName` = NEW.`StaffName`,
        `Password` = NEW.`Password`,
        `HandphoneNo` = NEW.`HandphoneNo`,
        `Age` = NEW.`Age`,
        `MarriageStatus` = NEW.`MarriageStatus`
    WHERE `StaffID` = NEW.`StaffID`;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_insert_staff_handphone`;
DELIMITER $$
CREATE TRIGGER `before_insert_staff_handphone` BEFORE INSERT ON `staff` FOR EACH ROW BEGIN
    IF NEW.HandphoneNo IS NOT NULL AND NEW.HandphoneNo <> '' THEN
        IF NOT (NEW.HandphoneNo REGEXP '^[0-9]{10,11}$') THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HandphoneNo must contain only digits and be 10 to 11 digits long';
        END IF;
    END IF;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_update_staff_handphone`;
DELIMITER $$
CREATE TRIGGER `before_update_staff_handphone` BEFORE UPDATE ON `staff` FOR EACH ROW BEGIN
    IF NEW.HandphoneNo IS NOT NULL AND NEW.HandphoneNo <> '' THEN
        IF NOT (NEW.HandphoneNo REGEXP '^[0-9]{10,11}$') THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HandphoneNo must contain only digits and be 10 to 11 digits long';
        END IF;
    END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `staff_backup`
--

DROP TABLE IF EXISTS `staff_backup`;
CREATE TABLE IF NOT EXISTS `staff_backup` (
  `StaffID` varchar(10) NOT NULL,
  `StaffName` varchar(100) NOT NULL,
  `Password` varchar(20) NOT NULL DEFAULT 'abc@123',
  `HandphoneNo` varchar(20) NOT NULL DEFAULT '',
  `Age` int(10) NOT NULL DEFAULT -1,
  `MarriageStatus` varchar(10) NOT NULL DEFAULT '-',
  PRIMARY KEY (`StaffID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `staff_backup`
--

INSERT INTO `staff_backup` (`StaffID`, `StaffName`, `Password`, `HandphoneNo`, `Age`, `MarriageStatus`) VALUES
('S001', 'Muhammad abu', 'abc1234', '', -1, '-'),
('S002', 'Liu Wei', 'def5678', '0123456789', 30, 'S'),
('S003', 'Aisha Kumar', 'ghi9012', '0112233445', 45, 'M'),
('S004', 'John Smith', 'jkl3456', '', 50, '-'),
('S005', 'Emily Tan', 'mno7890', '0109876543', -1, 'S');

--
-- Triggers `staff_backup`
--
DROP TRIGGER IF EXISTS `before_insert_staff_backup_handphone`;
DELIMITER $$
CREATE TRIGGER `before_insert_staff_backup_handphone` BEFORE INSERT ON `staff_backup` FOR EACH ROW BEGIN
    IF NEW.HandphoneNo IS NOT NULL AND NEW.HandphoneNo <> '' THEN
        IF NOT (NEW.HandphoneNo REGEXP '^[0-9]{10,11}$') THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HandphoneNo must contain only digits and be 10 to 11 digits long';
        END IF;
    END IF;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_update_staff_backup_handphone`;
DELIMITER $$
CREATE TRIGGER `before_update_staff_backup_handphone` BEFORE UPDATE ON `staff_backup` FOR EACH ROW BEGIN
    IF NEW.HandphoneNo IS NOT NULL AND NEW.HandphoneNo <> '' THEN
        IF NOT (NEW.HandphoneNo REGEXP '^[0-9]{10,11}$') THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'HandphoneNo must contain only digits and be 10 to 11 digits long';
        END IF;
    END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `supplier`
--

DROP TABLE IF EXISTS `supplier`;
CREATE TABLE IF NOT EXISTS `supplier` (
  `SupplierID` varchar(10) NOT NULL,
  `SupplierName` varchar(200) NOT NULL,
  `Address` varchar(300) NOT NULL,
  `TelephoneNumber` varchar(20) NOT NULL,
  `AdminID` varchar(10) NOT NULL,
  PRIMARY KEY (`SupplierID`),
  KEY `AdminID` (`AdminID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `supplier`
--

INSERT INTO `supplier` (`SupplierID`, `SupplierName`, `Address`, `TelephoneNumber`, `AdminID`) VALUES
('C001', 'Please Company', 'No. 10, Jalan Ampang, Kampung Baru, 50450 Kuala Lumpur, Wilayah Persekutuan', '6031234657', 'A001'),
('C002', 'Mega Supplies', 'No. 23, Jalan Bukit Bintang, 55100 Kuala Lumpur, Wilayah Persekutuan', '6031234586', 'A002'),
('C003', 'Elite Traders', 'No. 45, Lorong Perak, 11600 George Town, Pulau Pinang', '6031234596', 'A003'),
('C004', 'Prime Goods', 'No. 56, Jalan Raja Ekram, 30300 Ipoh, Perak', '6031234507', 'A001'),
('C005', 'Value Mart', 'No. 78, Jalan Temenggong, 75100 Bukit Cina Dalam, Melaka', '6031234571', 'A002');

--
-- Triggers `supplier`
--
DROP TRIGGER IF EXISTS `after_insert_supplier`;
DELIMITER $$
CREATE TRIGGER `after_insert_supplier` AFTER INSERT ON `supplier` FOR EACH ROW BEGIN
    INSERT INTO storehouse_management_system.supplier_backup 
    (`SupplierID`, `SupplierName`, `Address`, `TelephoneNumber`)
    VALUES 
    (NEW.`SupplierID`, NEW.`SupplierName`, NEW.`Address`, NEW.`TelephoneNumber`);
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `after_update_supplier`;
DELIMITER $$
CREATE TRIGGER `after_update_supplier` AFTER UPDATE ON `supplier` FOR EACH ROW BEGIN
    UPDATE storehouse_management_system.supplier_backup 
    SET 
        `SupplierName` = NEW.`SupplierName`,
        `Address` = NEW.`Address`,
        `TelephoneNumber` = NEW.`TelephoneNumber`
    WHERE `SupplierID` = NEW.`SupplierID`;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_insert_supplier_telephone`;
DELIMITER $$
CREATE TRIGGER `before_insert_supplier_telephone` BEFORE INSERT ON `supplier` FOR EACH ROW BEGIN
    IF NOT (NEW.TelephoneNumber REGEXP '^[0-9]{9,13}$') THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'TelephoneNumber must contain only digits and be 9 to 13 digits long';
    END IF;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_update_supplier_telephone`;
DELIMITER $$
CREATE TRIGGER `before_update_supplier_telephone` BEFORE UPDATE ON `supplier` FOR EACH ROW BEGIN
    IF NOT (NEW.TelephoneNumber REGEXP '^[0-9]{9,13}$') THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'TelephoneNumber must contain only digits and be 9 to 13 digits long';
    END IF;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `supplier_backup`
--

DROP TABLE IF EXISTS `supplier_backup`;
CREATE TABLE IF NOT EXISTS `supplier_backup` (
  `SupplierID` varchar(10) NOT NULL,
  `SupplierName` varchar(200) NOT NULL,
  `Address` varchar(300) NOT NULL,
  `TelephoneNumber` varchar(20) NOT NULL,
  PRIMARY KEY (`SupplierID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `supplier_backup`
--

INSERT INTO `supplier_backup` (`SupplierID`, `SupplierName`, `Address`, `TelephoneNumber`) VALUES
('C001', 'Please Company', 'No. 10, Jalan Ampang, Kampung Baru, 50450 Kuala Lumpur, Wilayah Persekutuan', '6031234657'),
('C002', 'Mega Supplies', 'No. 23, Jalan Bukit Bintang, 55100 Kuala Lumpur, Wilayah Persekutuan', '6031234586'),
('C003', 'Elite Traders', 'No. 45, Lorong Perak, 11600 George Town, Pulau Pinang', '6031234596'),
('C004', 'Prime Goods', 'No. 56, Jalan Raja Ekram, 30300 Ipoh, Perak', '6031234507'),
('C005', 'Value Mart', 'No. 78, Jalan Temenggong, 75100 Bukit Cina Dalam, Melaka', '6031234571');

--
-- Triggers `supplier_backup`
--
DROP TRIGGER IF EXISTS `before_insert_supplier_backup_telephone`;
DELIMITER $$
CREATE TRIGGER `before_insert_supplier_backup_telephone` BEFORE INSERT ON `supplier_backup` FOR EACH ROW BEGIN
    IF NOT (NEW.TelephoneNumber REGEXP '^[0-9]{9,13}$') THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'TelephoneNumber must contain only digits and be 9 to 13 digits long';
    END IF;
END
$$
DELIMITER ;
DROP TRIGGER IF EXISTS `before_update_supplier_backup_telephone`;
DELIMITER $$
CREATE TRIGGER `before_update_supplier_backup_telephone` BEFORE UPDATE ON `supplier_backup` FOR EACH ROW BEGIN
    IF NOT (NEW.TelephoneNumber REGEXP '^[0-9]{9,13}$') THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'TelephoneNumber must contain only digits and be 9 to 13 digits long';
    END IF;
END
$$
DELIMITER ;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `record_dispatch`
--
ALTER TABLE `record_dispatch`
  ADD CONSTRAINT `record_dispatch_ibfk_1` FOREIGN KEY (`ProductID`) REFERENCES `product_backup` (`ProductID`),
  ADD CONSTRAINT `record_dispatch_ibfk_2` FOREIGN KEY (`StaffID`) REFERENCES `staff_backup` (`StaffID`);

--
-- Constraints for table `record_restock`
--
ALTER TABLE `record_restock`
  ADD CONSTRAINT `record_restock_ibfk_1` FOREIGN KEY (`ProductID`) REFERENCES `product_backup` (`ProductID`),
  ADD CONSTRAINT `record_restock_ibfk_2` FOREIGN KEY (`SupplierID`) REFERENCES `supplier_backup` (`SupplierID`),
  ADD CONSTRAINT `record_restock_ibfk_3` FOREIGN KEY (`StaffID`) REFERENCES `staff_backup` (`StaffID`);

--
-- Constraints for table `staff`
--
ALTER TABLE `staff`
  ADD CONSTRAINT `staff_ibfk_1` FOREIGN KEY (`AdminID`) REFERENCES `admin` (`AdminID`);

--
-- Constraints for table `supplier`
--
ALTER TABLE `supplier`
  ADD CONSTRAINT `supplier_ibfk_1` FOREIGN KEY (`AdminID`) REFERENCES `admin` (`AdminID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
