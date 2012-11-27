-- phpMyAdmin SQL Dump
-- version 3.4.5
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Czas wygenerowania: 10 Lis 2012, 15:38
-- Wersja serwera: 5.5.16
-- Wersja PHP: 5.3.8

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Baza danych: `telemedycyna`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla  `data`
--

CREATE TABLE IF NOT EXISTS `data` (
  `session_id` int(11) NOT NULL,
  `start_date` datetime NOT NULL,
  `data_type` tinyint(4) NOT NULL,
  `file_path` varchar(128) COLLATE utf8_polish_ci DEFAULT NULL,
  `data_blob` blob,
  PRIMARY KEY (`session_id`,`start_date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

-- --------------------------------------------------------

--
-- Struktura tabeli dla  `patient_data`
--

CREATE TABLE IF NOT EXISTS `patient_data` (
  `PESEL` varchar(11) COLLATE utf8_polish_ci NOT NULL,
  `first_name` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  `last_name` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  PRIMARY KEY (`PESEL`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

-- --------------------------------------------------------

--
-- Struktura tabeli dla  `session`
--

CREATE TABLE IF NOT EXISTS `session` (
  `session_id` int(11) NOT NULL AUTO_INCREMENT,
  `device_name` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  `PESEL` varchar(11) COLLATE utf8_polish_ci NOT NULL,
  `start_date` datetime NOT NULL,
  `end_date` datetime,
  `sampling_frequency` int(11),
  `frame_size` int(11),
  PRIMARY KEY (`session_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

-- --------------------------------------------------------

--
-- Struktura tabeli dla  `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  `password` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  `privileges` int(11) DEFAULT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
