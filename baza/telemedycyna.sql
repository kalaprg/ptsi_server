-- phpMyAdmin SQL Dump
-- version 3.4.5
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Czas wygenerowania: 27 Lis 2012, 16:45
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

--
-- Zrzut danych tabeli `data`
--

INSERT INTO `data` (`session_id`, `start_date`, `data_type`, `file_path`, `data_blob`) VALUES
(1, '2012-11-27 16:06:08', 0, 'NULL', 0xd3e24c772d1a1bf152327533d80031cddcb509a18f5541f226d225d3c52dafaa269edddaa917aef5efc36ea73fdf383128cbba19686138200d85d375dafd0c28ec8484e19e1905d2a99dc6ea909deb4c3b6d4cffe17677e72cd0f8f49814d8991d6b63800deb2e7c3f2459bf6df5124ddba55ab0392e0cedcbdc1bb8c7171be4ff7d031dbf3531d9f97fae8c9f8fb41b519b6d943833fe1c32d1c4292fb1ddf1dbe2eb227d9e14d31ed74ebaef12d9a0e5780c13a62a555ba8d67caf978827a8f8c2a3f50dcee151f853bfb2222789c3de64c0954abe1a917613c9b2c94400f1a044b400376ea03acd1c8bdb0375cd6c86861082ed9554a8e5b1369a8b8e71b41990a5a9c5265778e659399c59a5ac832307844257570b873bd848cfa60e81d9accffe5a7d9d10f28b33525feb5e2ff1fe0ea43d50ad4716c0af89b9e26b3848a129487a297cd29004d25d8b82495d9c5fc4f9dca03f113dbe5f80c0064d9ee940c509718f6aa63a985de288e60d3e6d8d510d0a850eb470e47b0fb54ddc5ebca4d0cccca6781092d890fc6abddf53da4cd2dc5fa164b46df340e8b1c62a4399629b651683ab15a15234d97c9435c33a22748d819968e90f525512fb95b9a73d09e7b9716ff223d890cc19a074af0465d5bc8df7d3e3f85eea02b41b205130b183fdf064d213df417ba0e75ec517bf634ff20449b99c8c62a18ff79ad1b986fcb625317815d7f3e3f9f8693ff0339b3d765e1a00b25d4b24604302ff0fbb52268ca7a4339e0607c6f31bc7ca5bcc41fa3448f5a325ecddab1068fed2411d3c38ecca7220d668c2f352522a886fa958a270f7c0ba7c12ebff9744129cd1a8eb39bdd7c3c425c4cd71fce5baff9494893e12b4a1ce1e971d29df1f67e326240831e916bea9f4e4cf48db1c9eb933537cd401c7c06673431a30ce432432a8593a2957cf8c56a98e7181d63eff3eb2afd86f2778460d0ae3de8b1c4ef7720ebf0f2a27ef4a5453ae8b5b23d49504147b7046146b0f5b42d62f17f646827901d30d803699ce3f2969a33436bf67d6c5f45e6dcdb5d32697b4ff67c59d2d60924c1ce35b3da5755f449da5c37efd9cd98740f6a1ed0dd090e51622913d40504a9204fba0c3151d7da54355d0f99f7fd69be08899fc0580b6d13cf1620d0370af0d8d710c1367600bf65bed681895a7469704ccbbc8035f8fe1397c41f5bde94b267eeda716e365929eae1490c3263bb0e57569113a4f95a57bd46a35ddb8e2a643a1167ab2d0559a05047352e29b639decfb08a33a30498009d66447ad3a8446cebc958cd07857a99325b1b55d3c4994145d5177b1f0a4a6f108117e4d8cf6b52d9811e659219449b0d877c11c51121dc565eb9785d5cda19d2f1ae2a6f6617bcafd338e33143301c4e66d7f0722e9c067d97f0ba028ecf97316267b9ebfed1e0bb3a38e48995d5937183af607f7f4066282b4d9e8302b34dd84de521e31c1285a8108ee7ef73ada7134872f02175b761f28330b4737b0dc4140e96e0f9206211e300bb4bf78f10bdb02fb30e1cb47990493a4a9a483a3780913f1d7572be38fbe8853eaf262ab75db4c3f038d5be0f57a8aadb9e65d2d919cc59b96e04481fca4518997fffe8f9edad8fab0ed0f4d14ead7f945f70d76ae621e91b2902508ef375354c4cd94c610416fd53de82eb77eba402e54bedba557c3e67b3351159ff1df565d78466c775376d978b4680245b9548ef5ef230fe21527e400220d0a5e6e8904214bbb51cdcbede3de8a1c54d1204f1e89f51d0ef0576ef6e1c986a3469e3f0e7dc795d601ea68ee46b47e93e46fc882f8d7e667c729ec98a57e670316579b6632790fbe4efb4c01d89de10f835baae6a4425216dc831776f8c6cd1415c02672080830effd261f835ded4daff60c7fb456df6a04778b76f6a249c1bd8d02679bc71d015255956bdc9e4ecb5b84a9ce53d4563517e06991816ab04ae6c444e649365f88cfa06db773606661fb765a202be64ebb385e46f08019a3717703af2ce51e77cf4f24d98cb26edd1a781449fca4566f05503b4a1c5977c93d59926bc0d5470cdeb82bff71008f5cfbb8a69bea26eb621a45bd9c9c764ccbb672b99f2a8379c7bbf4fb07eec58a8b0ea4747b72196ccf03012d65f83d6050e4abc074f3557c5070e9dc1a8b6a28115a24c60f1de5a25dc66ad3ccb9187b01d0edee11a2b5174e05811860d1553197e721e37f72f255b56d6af6fa666e6486811e8d9ebf0d0a146980a2f60047ef6dc4be8114f4faa94c7fffe8df1687a6e486ff27e345d3bc394aeb36bd2276a0a341e62ea1963b70516285cbd749a42ac3aa53f1392c65334f994d24b6f0e4d19db5670ed28cbfb02a0606fd24bb4661c9998463d67bfdd61dfdaf54298f1be821c94a0beeff0ed2a0148a650e93f4b3bdfb9a64045b230f6d1f18ae5d2aa34ff825960c3f16ae3c7b341aebdffa3788697a9ede09f4604605a7901a9ce7026cea7053f80bfa2958830dbc77dd17964893bc97b82e6d3be1ae3c20802ceec2a9be5289b99bd2078d80487af15213995b04698ddabbee3d493086cec856686a6d3e75413637c7ce42398c5c418415c82a842a882cf8a1ece6532fa4da627d831f307e8357270108e177826950bc46ac1bdba791952cacd320dfdcaa9e985494295d185504b2a73cd102d2a90441cdcc97d087af00a13777609b37e7fd401625f28de9c12521e1f37aa22324465dd3b0072c72eb529d6b620c50e863dda1eabfe28799cea2bcd9b4441f1de3ff5680e1d1b7124c74696f1b8ec697262804f9e96115d18463cbb407974069ed1575fbe65f390cda4e4d6c2a4cc5eb39c4b16b2bd7fd39ec51969d74dbceaf4a72026d47177a0b9dc8908a18b0158ca39bf0670dac08780d5265ba8478929603fc45fd8d6341b59ecb7cf06b207d236c9309151848fe37f04f7f33bed651c71f9d85d43faa58b2e43d57c7ae06c7589d96439ed314a71d9b312d31ab5dd6eaa63c0c19e55385ab77086a3666c9b9d23c8be82ac676cbd318deb7dcd176e8a6a1c020f508c891486012fd661dd11c3a54593ee07ad731006f40655bba874ea317130d0a902f704430a645587f5a86b737eaea8ee976a0a24da63e7ed7eefad18a101c1211e2b3650c5187c2a8a650547208251f6d4237e661c7bf4c77f335390394c37fa1a9f9be836ac285098897b992b5f23b936706c5e6d9ad156453a24eb5acf09b670daa153ef7dde7cb88c3af40fa3928836c730781e5ef22c596a7d7495f8bac8c952e33e403c031162e3cfe24dee22f66d9e873dcf26dbbc342ae9e66b67cfb37caeee01e9353aa11036a6ee569d6986437ccd29f080f2886e11e69b5b7abd0913250e394b17e9ce58eb6c72ca3fbaba40d87ec722fbdb035fa5f01de6c0173c257bd06ed64294db95786d0a0953ab04fe38189fb705f9cf816d92e88dd642bf1c19e0fd2b53c06b6e74352ea467cee8c422171e5d2dd34f9bcf4b75b11bd4146f659c7eaaa9f0f04c55616b1bea7ae84b0d16bd65e1fc05d7418660110f5fdea8058f6de4e3ab0ca7925e19e06a4385eb7f941761b0d884fc7ad011fe2f2a705e7016455a6f531318c1a7e59255cd443dfffb96f1190d4f53b1b5160909e87beb1bed8f8bcb9d960859cb38923b6ca5baf4c9c66488eeb775aca1e6920e0e53e68f61c6f976cf27fec124ad6099961cdaf48b106c9f016feac2230e59d9f08ad9310f6613ff8c2b84ae609eadeefe2c2675bc2f49b9f9db2b88c4e7a0cc53932bb762f76bd439231267fb96aaa9f956160ee95dbc682c8ba066e42d07739c0f5678f8f571837a912084522017591c2d32167f4615380de3e1e31d290e5e3e51f1fc3ddac58bb518478a00b5548adb13f3230a52e49832968893ec7625fabf0d86c7ae31c86b6d0affff83d0e2adbaace1caf70ebb072ef5ecefce8edec792847d41c6750a8126fba24b581d52403f4a8664e1f409034794875d2ffda9435d695e11132f0c9b676cd703f9d523d0dd8f4987478d4f2b30f4f062ad2d05fc742e91bc20ca3ee8a2d126aff592c7de19edb3b884550ddd6f99b0d0a6b2b785617b46c0995bc112176dbae9a5b7f0bec678e84d6f44c5b5a857e5309fcd07a800e744633f30ce36962cacc88bcd5999388ef17f11f569e70a8e79f8947897de692c99e2145c2c1cf364092bd8b257b792a0bbe14712ac127287ca814b320298e8cfd9c5cf1a75259da8628c02e355ca636289fc852926b87eb859631a85b2882e5566a6b61833f1136e4b7190fcde223958fe2942a544499da4a91698e041fca831d2dd6fb1e2fff33a1675194b7b5f72d88b0016139f564dd13c042cee1b96c50991026293fcb2ab7075fef27ba7e2ae85a31965f0926f0dd274b46cebb9c72331438e252b0bb9aab5cc87b2f9de5c5ad4f8cdb8d8c7e46c0cf5bfdf75970e313d4d092dd37a657f8e9c97d142991cf63adea585189a66d705318eeb185dec8da22febe349d356a84f68b7e3d262b50f4bd729614ca466dd3fba9a165058d286f46eaefbea90fda39cb3606145c5a5147c74d1c3f15054fd30e369e79a2270bba88beace325dcaffeaff19b0092c7d84413a27238338b1a57d6e96e5724517aa3309f7e0bd746f1baece4c769921af0d3e89a2169ec81d64699e8986553b57b1285b7ec9e89f3a957c83811b722fa9edc905db038078f8c8aa05ba40df7fb19efd02e7e04018bdfbc8c1501530b0fec475112a70f4e5c430d0a336b1ab6d7cb5922592cbca608b799d17fa220a31402e1fc74e18f55879103ae9a3f83c7bccb459368d82fea651fe17eeb08f510077b85e440f3ee727bd5d45e1c0167aa6ea6e42b455a5e39e26abeab166e0903ab21e1e5ff704fcef2d1bf0202d64cc6b5023ed038dbd1fc4540394049fde2e2c79dfce8f78037d1b42dcb8295e8bc8359885c6b88831199574b5825ccdd0d0a162496dfd6724930e3889001c61a5f3e4921df45e57e79a57ae0302543e73509088ab59095b6f6c16c37a267b7dc635b80d6af012462e61505a89aa880e22d651fc0a2190f0bad73d48a0c083b221a999040a299127cf1eaa4762fe8c504efa8c29004812abb96cc834676e93b33af),
(2, '2012-11-27 16:10:19', 1, 'NULL', 0xd3e24c772d1a1bf152327533d80031cddcb509a18f5541f226d225d3c52dafaa269edddaa917aef5efc36ea73fdf383128cbba19686138200d85d375dafd0c28ec8484e19e1905d2a99dc6ea909deb4c3b6d4cffe17677e72cd0f8f49814d8991d6b63800deb2e7c3f2459bf6df5124ddba55ab0392e0cedcbdc1bb8c7171be4ff7d031dbf3531d9f97fae8c9f8fb41b519b6d943833fe1c32d1c4292fb1ddf1dbe2eb227d9e14d31ed74ebaef12d9a0e5780c13a62a555ba8d67caf978827a8f8c2a3f50dcee151f853bfb2222789c3de64c0954abe1a917613c9b2c94400f1a044b400376ea03acd1c8bdb0375cd6c86861082ed9554a8e5b1369a8b8e71b41990a5a9c5265778e659399c59a5ac832307844257570b873bd848cfa60e81d9accffe5a7d9d10f28b33525feb5e2ff1fe0ea43d50ad4716c0af89b9e26b3848a129487a297cd29004d25d8b82495d9c5fc4f9dca03f113dbe5f80c0064d9ee940c509718f6aa63a985de288e60d3e6d8d510d0a850eb470e47b0fb54ddc5ebca4d0cccca6781092d890fc6abddf53da4cd2dc5fa164b46df340e8b1c62a4399629b651683ab15a15234d97c9435c33a22748d819968e90f525512fb95b9a73d09e7b9716ff223d890cc19a074af0465d5bc8df7d3e3f85eea02b41b205130b183fdf064d213df417ba0e75ec517bf634ff20449b99c8c62a18ff79ad1b986fcb625317815d7f3e3f9f8693ff0339b3d765e1a00b25d4b24604302ff0fbb52268ca7a4339e0607c6f31bc7ca5bcc41fa3448f5a325ecddab1068fed2411d3c38ecca7220d668c2f352522a886fa958a270f7c0ba7c12ebff9744129cd1a8eb39bdd7c3c425c4cd71fce5baff9494893e12b4a1ce1e971d29df1f67e326240831e916bea9f4e4cf48db1c9eb933537cd401c7c06673431a30ce432432a8593a2957cf8c56a98e7181d63eff3eb2afd86f2778460d0ae3de8b1c4ef7720ebf0f2a27ef4a5453ae8b5b23d49504147b7046146b0f5b42d62f17f646827901d30d803699ce3f2969a33436bf67d6c5f45e6dcdb5d32697b4ff67c59d2d60924c1ce35b3da5755f449da5c37efd9cd98740f6a1ed0dd090e51622913d40504a9204fba0c3151d7da54355d0f99f7fd69be08899fc0580b6d13cf1620d0370af0d8d710c1367600bf65bed681895a7469704ccbbc8035f8fe1397c41f5bde94b267eeda716e365929eae1490c3263bb0e57569113a4f95a57bd46a35ddb8e2a643a1167ab2d0559a05047352e29b639decfb08a33a30498009d66447ad3a8446cebc958cd07857a99325b1b55d3c4994145d5177b1f0a4a6f108117e4d8cf6b52d9811e659219449b0d877c11c51121dc565eb9785d5cda19d2f1ae2a6f6617bcafd338e33143301c4e66d7f0722e9c067d97f0ba028ecf97316267b9ebfed1e0bb3a38e48995d5937183af607f7f4066282b4d9e8302b34dd84de521e31c1285a8108ee7ef73ada7134872f02175b761f28330b4737b0dc4140e96e0f9206211e300bb4bf78f10bdb02fb30e1cb47990493a4a9a483a3780913f1d7572be38fbe8853eaf262ab75db4c3f038d5be0f57a8aadb9e65d2d919cc59b96e04481fca4518997fffe8f9edad8fab0ed0f4d14ead7f945f70d76ae621e91b2902508ef375354c4cd94c610416fd53de82eb77eba402e54bedba557c3e67b3351159ff1df565d78466c775376d978b4680245b9548ef5ef230fe21527e400220d0a5e6e8904214bbb51cdcbede3de8a1c54d1204f1e89f51d0ef0576ef6e1c986a3469e3f0e7dc795d601ea68ee46b47e93e46fc882f8d7e667c729ec98a57e670316579b6632790fbe4efb4c01d89de10f835baae6a4425216dc831776f8c6cd1415c02672080830effd261f835ded4daff60c7fb456df6a04778b76f6a249c1bd8d02679bc71d015255956bdc9e4ecb5b84a9ce53d4563517e06991816ab04ae6c444e649365f88cfa06db773606661fb765a202be64ebb385e46f08019a3717703af2ce51e77cf4f24d98cb26edd1a781449fca4566f05503b4a1c5977c93d59926bc0d5470cdeb82bff71008f5cfbb8a69bea26eb621a45bd9c9c764ccbb672b99f2a8379c7bbf4fb07eec58a8b0ea4747b72196ccf03012d65f83d6050e4abc074f3557c5070e9dc1a8b6a28115a24c60f1de5a25dc66ad3ccb9187b01d0edee11a2b5174e05811860d1553197e721e37f72f255b56d6af6fa666e6486811e8d9ebf0d0a146980a2f60047ef6dc4be8114f4faa94c7fffe8df1687a6e486ff27e345d3bc394aeb36bd2276a0a341e62ea1963b70516285cbd749a42ac3aa53f1392c65334f994d24b6f0e4d19db5670ed28cbfb02a0606fd24bb4661c9998463d67bfdd61dfdaf54298f1be821c94a0beeff0ed2a0148a650e93f4b3bdfb9a64045b230f6d1f18ae5d2aa34ff825960c3f16ae3c7b341aebdffa3788697a9ede09f4604605a7901a9ce7026cea7053f80bfa2958830dbc77dd17964893bc97b82e6d3be1ae3c20802ceec2a9be5289b99bd2078d80487af15213995b04698ddabbee3d493086cec856686a6d3e75413637c7ce42398c5c418415c82a842a882cf8a1ece6532fa4da627d831f307e8357270108e177826950bc46ac1bdba791952cacd320dfdcaa9e985494295d185504b2a73cd102d2a90441cdcc97d087af00a13777609b37e7fd401625f28de9c12521e1f37aa22324465dd3b0072c72eb529d6b620c50e863dda1eabfe28799cea2bcd9b4441f1de3ff5680e1d1b7124c74696f1b8ec697262804f9e96115d18463cbb407974069ed1575fbe65f390cda4e4d6c2a4cc5eb39c4b16b2bd7fd39ec51969d74dbceaf4a72026d47177a0b9dc8908a18b0158ca39bf0670dac08780d5265ba8478929603fc45fd8d6341b59ecb7cf06b207d236c9309151848fe37f04f7f33bed651c71f9d85d43faa58b2e43d57c7ae06c7589d96439ed314a71d9b312d31ab5dd6eaa63c0c19e55385ab77086a3666c9b9d23c8be82ac676cbd318deb7dcd176e8a6a1c020f508c891486012fd661dd11c3a54593ee07ad731006f40655bba874ea317130d0a902f704430a645587f5a86b737eaea8ee976a0a24da63e7ed7eefad18a101c1211e2b3650c5187c2a8a650547208251f6d4237e661c7bf4c77f335390394c37fa1a9f9be836ac285098897b992b5f23b936706c5e6d9ad156453a24eb5acf09b670daa153ef7dde7cb88c3af40fa3928836c730781e5ef22c596a7d7495f8bac8c952e33e403c031162e3cfe24dee22f66d9e873dcf26dbbc342ae9e66b67cfb37caeee01e9353aa11036a6ee569d6986437ccd29f080f2886e11e69b5b7abd0913250e394b17e9ce58eb6c72ca3fbaba40d87ec722fbdb035fa5f01de6c0173c257bd06ed64294db95786d0a0953ab04fe38189fb705f9cf816d92e88dd642bf1c19e0fd2b53c06b6e74352ea467cee8c422171e5d2dd34f9bcf4b75b11bd4146f659c7eaaa9f0f04c55616b1bea7ae84b0d16bd65e1fc05d7418660110f5fdea8058f6de4e3ab0ca7925e19e06a4385eb7f941761b0d884fc7ad011fe2f2a705e7016455a6f531318c1a7e59255cd443dfffb96f1190d4f53b1b5160909e87beb1bed8f8bcb9d960859cb38923b6ca5baf4c9c66488eeb775aca1e6920e0e53e68f61c6f976cf27fec124ad6099961cdaf48b106c9f016feac2230e59d9f08ad9310f6613ff8c2b84ae609eadeefe2c2675bc2f49b9f9db2b88c4e7a0cc53932bb762f76bd439231267fb96aaa9f956160ee95dbc682c8ba066e42d07739c0f5678f8f571837a912084522017591c2d32167f4615380de3e1e31d290e5e3e51f1fc3ddac58bb518478a00b5548adb13f3230a52e49832968893ec7625fabf0d86c7ae31c86b6d0affff83d0e2adbaace1caf70ebb072ef5ecefce8edec792847d41c6750a8126fba24b581d52403f4a8664e1f409034794875d2ffda9435d695e11132f0c9b676cd703f9d523d0dd8f4987478d4f2b30f4f062ad2d05fc742e91bc20ca3ee8a2d126aff592c7de19edb3b884550ddd6f99b0d0a6b2b785617b46c0995bc112176dbae9a5b7f0bec678e84d6f44c5b5a857e5309fcd07a800e744633f30ce36962cacc88bcd5999388ef17f11f569e70a8e79f8947897de692c99e2145c2c1cf364092bd8b257b792a0bbe14712ac127287ca814b320298e8cfd9c5cf1a75259da8628c02e355ca636289fc852926b87eb859631a85b2882e5566a6b61833f1136e4b7190fcde223958fe2942a544499da4a91698e041fca831d2dd6fb1e2fff33a1675194b7b5f72d88b0016139f564dd13c042cee1b96c50991026293fcb2ab7075fef27ba7e2ae85a31965f0926f0dd274b46cebb9c72331438e252b0bb9aab5cc87b2f9de5c5ad4f8cdb8d8c7e46c0cf5bfdf75970e313d4d092dd37a657f8e9c97d142991cf63adea585189a66d705318eeb185dec8da22febe349d356a84f68b7e3d262b50f4bd729614ca466dd3fba9a165058d286f46eaefbea90fda39cb3606145c5a5147c74d1c3f15054fd30e369e79a2270bba88beace325dcaffeaff19b0092c7d84413a27238338b1a57d6e96e5724517aa3309f7e0bd746f1baece4c769921af0d3e89a2169ec81d64699e8986553b57b1285b7ec9e89f3a957c83811b722fa9edc905db038078f8c8aa05ba40df7fb19efd02e7e04018bdfbc8c1501530b0fec475112a70f4e5c430d0a336b1ab6d7cb5922592cbca608b799d17fa220a31402e1fc74e18f55879103ae9a3f83c7bccb459368d82fea651fe17eeb08f510077b85e440f3ee727bd5d45e1c0167aa6ea6e42b455a5e39e26abeab166e0903ab21e1e5ff704fcef2d1bf0202d64cc6b5023ed038dbd1fc4540394049fde2e2c79dfce8f78037d1b42dcb8295e8bc8359885c6b88831199574b5825ccdd0d0a162496dfd6724930e3889001c61a5f3e4921df45e57e79a57ae0302543e73509088ab59095b6f6c16c37a267b7dc635b80d6af012462e61505a89aa880e22d651fc0a2190f0bad73d48a0c083b221a999040a299127cf1eaa4762fe8c504efa8c29004812abb96cc834676e93b33af),
(3, '2012-11-27 16:10:19', 2, 'NULL', 0xd3e24c772d1a1bf152327533d80031cddcb509a18f5541f226d225d3c52dafaa269edddaa917aef5efc36ea73fdf383128cbba19686138200d85d375dafd0c28ec8484e19e1905d2a99dc6ea909deb4c3b6d4cffe17677e72cd0f8f49814d8991d6b63800deb2e7c3f2459bf6df5124ddba55ab0392e0cedcbdc1bb8c7171be4ff7d031dbf3531d9f97fae8c9f8fb41b519b6d943833fe1c32d1c4292fb1ddf1dbe2eb227d9e14d31ed74ebaef12d9a0e5780c13a62a555ba8d67caf978827a8f8c2a3f50dcee151f853bfb2222789c3de64c0954abe1a917613c9b2c94400f1a044b400376ea03acd1c8bdb0375cd6c86861082ed9554a8e5b1369a8b8e71b41990a5a9c5265778e659399c59a5ac832307844257570b873bd848cfa60e81d9accffe5a7d9d10f28b33525feb5e2ff1fe0ea43d50ad4716c0af89b9e26b3848a129487a297cd29004d25d8b82495d9c5fc4f9dca03f113dbe5f80c0064d9ee940c509718f6aa63a985de288e60d3e6d8d510d0a850eb470e47b0fb54ddc5ebca4d0cccca6781092d890fc6abddf53da4cd2dc5fa164b46df340e8b1c62a4399629b651683ab15a15234d97c9435c33a22748d819968e90f525512fb95b9a73d09e7b9716ff223d890cc19a074af0465d5bc8df7d3e3f85eea02b41b205130b183fdf064d213df417ba0e75ec517bf634ff20449b99c8c62a18ff79ad1b986fcb625317815d7f3e3f9f8693ff0339b3d765e1a00b25d4b24604302ff0fbb52268ca7a4339e0607c6f31bc7ca5bcc41fa3448f5a325ecddab1068fed2411d3c38ecca7220d668c2f352522a886fa958a270f7c0ba7c12ebff9744129cd1a8eb39bdd7c3c425c4cd71fce5baff9494893e12b4a1ce1e971d29df1f67e326240831e916bea9f4e4cf48db1c9eb933537cd401c7c06673431a30ce432432a8593a2957cf8c56a98e7181d63eff3eb2afd86f2778460d0ae3de8b1c4ef7720ebf0f2a27ef4a5453ae8b5b23d49504147b7046146b0f5b42d62f17f646827901d30d803699ce3f2969a33436bf67d6c5f45e6dcdb5d32697b4ff67c59d2d60924c1ce35b3da5755f449da5c37efd9cd98740f6a1ed0dd090e51622913d40504a9204fba0c3151d7da54355d0f99f7fd69be08899fc0580b6d13cf1620d0370af0d8d710c1367600bf65bed681895a7469704ccbbc8035f8fe1397c41f5bde94b267eeda716e365929eae1490c3263bb0e57569113a4f95a57bd46a35ddb8e2a643a1167ab2d0559a05047352e29b639decfb08a33a30498009d66447ad3a8446cebc958cd07857a99325b1b55d3c4994145d5177b1f0a4a6f108117e4d8cf6b52d9811e659219449b0d877c11c51121dc565eb9785d5cda19d2f1ae2a6f6617bcafd338e33143301c4e66d7f0722e9c067d97f0ba028ecf97316267b9ebfed1e0bb3a38e48995d5937183af607f7f4066282b4d9e8302b34dd84de521e31c1285a8108ee7ef73ada7134872f02175b761f28330b4737b0dc4140e96e0f9206211e300bb4bf78f10bdb02fb30e1cb47990493a4a9a483a3780913f1d7572be38fbe8853eaf262ab75db4c3f038d5be0f57a8aadb9e65d2d919cc59b96e04481fca4518997fffe8f9edad8fab0ed0f4d14ead7f945f70d76ae621e91b2902508ef375354c4cd94c610416fd53de82eb77eba402e54bedba557c3e67b3351159ff1df565d78466c775376d978b4680245b9548ef5ef230fe21527e400220d0a5e6e8904214bbb51cdcbede3de8a1c54d1204f1e89f51d0ef0576ef6e1c986a3469e3f0e7dc795d601ea68ee46b47e93e46fc882f8d7e667c729ec98a57e670316579b6632790fbe4efb4c01d89de10f835baae6a4425216dc831776f8c6cd1415c02672080830effd261f835ded4daff60c7fb456df6a04778b76f6a249c1bd8d02679bc71d015255956bdc9e4ecb5b84a9ce53d4563517e06991816ab04ae6c444e649365f88cfa06db773606661fb765a202be64ebb385e46f08019a3717703af2ce51e77cf4f24d98cb26edd1a781449fca4566f05503b4a1c5977c93d59926bc0d5470cdeb82bff71008f5cfbb8a69bea26eb621a45bd9c9c764ccbb672b99f2a8379c7bbf4fb07eec58a8b0ea4747b72196ccf03012d65f83d6050e4abc074f3557c5070e9dc1a8b6a28115a24c60f1de5a25dc66ad3ccb9187b01d0edee11a2b5174e05811860d1553197e721e37f72f255b56d6af6fa666e6486811e8d9ebf0d0a146980a2f60047ef6dc4be8114f4faa94c7fffe8df1687a6e486ff27e345d3bc394aeb36bd2276a0a341e62ea1963b70516285cbd749a42ac3aa53f1392c65334f994d24b6f0e4d19db5670ed28cbfb02a0606fd24bb4661c9998463d67bfdd61dfdaf54298f1be821c94a0beeff0ed2a0148a650e93f4b3bdfb9a64045b230f6d1f18ae5d2aa34ff825960c3f16ae3c7b341aebdffa3788697a9ede09f4604605a7901a9ce7026cea7053f80bfa2958830dbc77dd17964893bc97b82e6d3be1ae3c20802ceec2a9be5289b99bd2078d80487af15213995b04698ddabbee3d493086cec856686a6d3e75413637c7ce42398c5c418415c82a842a882cf8a1ece6532fa4da627d831f307e8357270108e177826950bc46ac1bdba791952cacd320dfdcaa9e985494295d185504b2a73cd102d2a90441cdcc97d087af00a13777609b37e7fd401625f28de9c12521e1f37aa22324465dd3b0072c72eb529d6b620c50e863dda1eabfe28799cea2bcd9b4441f1de3ff5680e1d1b7124c74696f1b8ec697262804f9e96115d18463cbb407974069ed1575fbe65f390cda4e4d6c2a4cc5eb39c4b16b2bd7fd39ec51969d74dbceaf4a72026d47177a0b9dc8908a18b0158ca39bf0670dac08780d5265ba8478929603fc45fd8d6341b59ecb7cf06b207d236c9309151848fe37f04f7f33bed651c71f9d85d43faa58b2e43d57c7ae06c7589d96439ed314a71d9b312d31ab5dd6eaa63c0c19e55385ab77086a3666c9b9d23c8be82ac676cbd318deb7dcd176e8a6a1c020f508c891486012fd661dd11c3a54593ee07ad731006f40655bba874ea317130d0a902f704430a645587f5a86b737eaea8ee976a0a24da63e7ed7eefad18a101c1211e2b3650c5187c2a8a650547208251f6d4237e661c7bf4c77f335390394c37fa1a9f9be836ac285098897b992b5f23b936706c5e6d9ad156453a24eb5acf09b670daa153ef7dde7cb88c3af40fa3928836c730781e5ef22c596a7d7495f8bac8c952e33e403c031162e3cfe24dee22f66d9e873dcf26dbbc342ae9e66b67cfb37caeee01e9353aa11036a6ee569d6986437ccd29f080f2886e11e69b5b7abd0913250e394b17e9ce58eb6c72ca3fbaba40d87ec722fbdb035fa5f01de6c0173c257bd06ed64294db95786d0a0953ab04fe38189fb705f9cf816d92e88dd642bf1c19e0fd2b53c06b6e74352ea467cee8c422171e5d2dd34f9bcf4b75b11bd4146f659c7eaaa9f0f04c55616b1bea7ae84b0d16bd65e1fc05d7418660110f5fdea8058f6de4e3ab0ca7925e19e06a4385eb7f941761b0d884fc7ad011fe2f2a705e7016455a6f531318c1a7e59255cd443dfffb96f1190d4f53b1b5160909e87beb1bed8f8bcb9d960859cb38923b6ca5baf4c9c66488eeb775aca1e6920e0e53e68f61c6f976cf27fec124ad6099961cdaf48b106c9f016feac2230e59d9f08ad9310f6613ff8c2b84ae609eadeefe2c2675bc2f49b9f9db2b88c4e7a0cc53932bb762f76bd439231267fb96aaa9f956160ee95dbc682c8ba066e42d07739c0f5678f8f571837a912084522017591c2d32167f4615380de3e1e31d290e5e3e51f1fc3ddac58bb518478a00b5548adb13f3230a52e49832968893ec7625fabf0d86c7ae31c86b6d0affff83d0e2adbaace1caf70ebb072ef5ecefce8edec792847d41c6750a8126fba24b581d52403f4a8664e1f409034794875d2ffda9435d695e11132f0c9b676cd703f9d523d0dd8f4987478d4f2b30f4f062ad2d05fc742e91bc20ca3ee8a2d126aff592c7de19edb3b884550ddd6f99b0d0a6b2b785617b46c0995bc112176dbae9a5b7f0bec678e84d6f44c5b5a857e5309fcd07a800e744633f30ce36962cacc88bcd5999388ef17f11f569e70a8e79f8947897de692c99e2145c2c1cf364092bd8b257b792a0bbe14712ac127287ca814b320298e8cfd9c5cf1a75259da8628c02e355ca636289fc852926b87eb859631a85b2882e5566a6b61833f1136e4b7190fcde223958fe2942a544499da4a91698e041fca831d2dd6fb1e2fff33a1675194b7b5f72d88b0016139f564dd13c042cee1b96c50991026293fcb2ab7075fef27ba7e2ae85a31965f0926f0dd274b46cebb9c72331438e252b0bb9aab5cc87b2f9de5c5ad4f8cdb8d8c7e46c0cf5bfdf75970e313d4d092dd37a657f8e9c97d142991cf63adea585189a66d705318eeb185dec8da22febe349d356a84f68b7e3d262b50f4bd729614ca466dd3fba9a165058d286f46eaefbea90fda39cb3606145c5a5147c74d1c3f15054fd30e369e79a2270bba88beace325dcaffeaff19b0092c7d84413a27238338b1a57d6e96e5724517aa3309f7e0bd746f1baece4c769921af0d3e89a2169ec81d64699e8986553b57b1285b7ec9e89f3a957c83811b722fa9edc905db038078f8c8aa05ba40df7fb19efd02e7e04018bdfbc8c1501530b0fec475112a70f4e5c430d0a336b1ab6d7cb5922592cbca608b799d17fa220a31402e1fc74e18f55879103ae9a3f83c7bccb459368d82fea651fe17eeb08f510077b85e440f3ee727bd5d45e1c0167aa6ea6e42b455a5e39e26abeab166e0903ab21e1e5ff704fcef2d1bf0202d64cc6b5023ed038dbd1fc4540394049fde2e2c79dfce8f78037d1b42dcb8295e8bc8359885c6b88831199574b5825ccdd0d0a162496dfd6724930e3889001c61a5f3e4921df45e57e79a57ae0302543e73509088ab59095b6f6c16c37a267b7dc635b80d6af012462e61505a89aa880e22d651fc0a2190f0bad73d48a0c083b221a999040a299127cf1eaa4762fe8c504efa8c29004812abb96cc834676e93b33af),
(4, '2012-11-27 16:17:16', 1, 'NULL', 0xd3e24c772d1a1bf152327533d80031cddcb509a18f5541f226d225d3c52dafaa269edddaa917aef5efc36ea73fdf383128cbba19686138200d85d375dafd0c28ec8484e19e1905d2a99dc6ea909deb4c3b6d4cffe17677e72cd0f8f49814d8991d6b63800deb2e7c3f2459bf6df5124ddba55ab0392e0cedcbdc1bb8c7171be4ff7d031dbf3531d9f97fae8c9f8fb41b519b6d943833fe1c32d1c4292fb1ddf1dbe2eb227d9e14d31ed74ebaef12d9a0e5780c13a62a555ba8d67caf978827a8f8c2a3f50dcee151f853bfb2222789c3de64c0954abe1a917613c9b2c94400f1a044b400376ea03acd1c8bdb0375cd6c86861082ed9554a8e5b1369a8b8e71b41990a5a9c5265778e659399c59a5ac832307844257570b873bd848cfa60e81d9accffe5a7d9d10f28b33525feb5e2ff1fe0ea43d50ad4716c0af89b9e26b3848a129487a297cd29004d25d8b82495d9c5fc4f9dca03f113dbe5f80c0064d9ee940c509718f6aa63a985de288e60d3e6d8d510d0a850eb470e47b0fb54ddc5ebca4d0cccca6781092d890fc6abddf53da4cd2dc5fa164b46df340e8b1c62a4399629b651683ab15a15234d97c9435c33a22748d819968e90f525512fb95b9a73d09e7b9716ff223d890cc19a074af0465d5bc8df7d3e3f85eea02b41b205130b183fdf064d213df417ba0e75ec517bf634ff20449b99c8c62a18ff79ad1b986fcb625317815d7f3e3f9f8693ff0339b3d765e1a00b25d4b24604302ff0fbb52268ca7a4339e0607c6f31bc7ca5bcc41fa3448f5a325ecddab1068fed2411d3c38ecca7220d668c2f352522a886fa958a270f7c0ba7c12ebff9744129cd1a8eb39bdd7c3c425c4cd71fce5baff9494893e12b4a1ce1e971d29df1f67e326240831e916bea9f4e4cf48db1c9eb933537cd401c7c06673431a30ce432432a8593a2957cf8c56a98e7181d63eff3eb2afd86f2778460d0ae3de8b1c4ef7720ebf0f2a27ef4a5453ae8b5b23d49504147b7046146b0f5b42d62f17f646827901d30d803699ce3f2969a33436bf67d6c5f45e6dcdb5d32697b4ff67c59d2d60924c1ce35b3da5755f449da5c37efd9cd98740f6a1ed0dd090e51622913d40504a9204fba0c3151d7da54355d0f99f7fd69be08899fc0580b6d13cf1620d0370af0d8d710c1367600bf65bed681895a7469704ccbbc8035f8fe1397c41f5bde94b267eeda716e365929eae1490c3263bb0e57569113a4f95a57bd46a35ddb8e2a643a1167ab2d0559a05047352e29b639decfb08a33a30498009d66447ad3a8446cebc958cd07857a99325b1b55d3c4994145d5177b1f0a4a6f108117e4d8cf6b52d9811e659219449b0d877c11c51121dc565eb9785d5cda19d2f1ae2a6f6617bcafd338e33143301c4e66d7f0722e9c067d97f0ba028ecf97316267b9ebfed1e0bb3a38e48995d5937183af607f7f4066282b4d9e8302b34dd84de521e31c1285a8108ee7ef73ada7134872f02175b761f28330b4737b0dc4140e96e0f9206211e300bb4bf78f10bdb02fb30e1cb47990493a4a9a483a3780913f1d7572be38fbe8853eaf262ab75db4c3f038d5be0f57a8aadb9e65d2d919cc59b96e04481fca4518997fffe8f9edad8fab0ed0f4d14ead7f945f70d76ae621e91b2902508ef375354c4cd94c610416fd53de82eb77eba402e54bedba557c3e67b3351159ff1df565d78466c775376d978b4680245b9548ef5ef230fe21527e400220d0a5e6e8904214bbb51cdcbede3de8a1c54d1204f1e89f51d0ef0576ef6e1c986a3469e3f0e7dc795d601ea68ee46b47e93e46fc882f8d7e667c729ec98a57e670316579b6632790fbe4efb4c01d89de10f835baae6a4425216dc831776f8c6cd1415c02672080830effd261f835ded4daff60c7fb456df6a04778b76f6a249c1bd8d02679bc71d015255956bdc9e4ecb5b84a9ce53d4563517e06991816ab04ae6c444e649365f88cfa06db773606661fb765a202be64ebb385e46f08019a3717703af2ce51e77cf4f24d98cb26edd1a781449fca4566f05503b4a1c5977c93d59926bc0d5470cdeb82bff71008f5cfbb8a69bea26eb621a45bd9c9c764ccbb672b99f2a8379c7bbf4fb07eec58a8b0ea4747b72196ccf03012d65f83d6050e4abc074f3557c5070e9dc1a8b6a28115a24c60f1de5a25dc66ad3ccb9187b01d0edee11a2b5174e05811860d1553197e721e37f72f255b56d6af6fa666e6486811e8d9ebf0d0a146980a2f60047ef6dc4be8114f4faa94c7fffe8df1687a6e486ff27e345d3bc394aeb36bd2276a0a341e62ea1963b70516285cbd749a42ac3aa53f1392c65334f994d24b6f0e4d19db5670ed28cbfb02a0606fd24bb4661c9998463d67bfdd61dfdaf54298f1be821c94a0beeff0ed2a0148a650e93f4b3bdfb9a64045b230f6d1f18ae5d2aa34ff825960c3f16ae3c7b341aebdffa3788697a9ede09f4604605a7901a9ce7026cea7053f80bfa2958830dbc77dd17964893bc97b82e6d3be1ae3c20802ceec2a9be5289b99bd2078d80487af15213995b04698ddabbee3d493086cec856686a6d3e75413637c7ce42398c5c418415c82a842a882cf8a1ece6532fa4da627d831f307e8357270108e177826950bc46ac1bdba791952cacd320dfdcaa9e985494295d185504b2a73cd102d2a90441cdcc97d087af00a13777609b37e7fd401625f28de9c12521e1f37aa22324465dd3b0072c72eb529d6b620c50e863dda1eabfe28799cea2bcd9b4441f1de3ff5680e1d1b7124c74696f1b8ec697262804f9e96115d18463cbb407974069ed1575fbe65f390cda4e4d6c2a4cc5eb39c4b16b2bd7fd39ec51969d74dbceaf4a72026d47177a0b9dc8908a18b0158ca39bf0670dac08780d5265ba8478929603fc45fd8d6341b59ecb7cf06b207d236c9309151848fe37f04f7f33bed651c71f9d85d43faa58b2e43d57c7ae06c7589d96439ed314a71d9b312d31ab5dd6eaa63c0c19e55385ab77086a3666c9b9d23c8be82ac676cbd318deb7dcd176e8a6a1c020f508c891486012fd661dd11c3a54593ee07ad731006f40655bba874ea317130d0a902f704430a645587f5a86b737eaea8ee976a0a24da63e7ed7eefad18a101c1211e2b3650c5187c2a8a650547208251f6d4237e661c7bf4c77f335390394c37fa1a9f9be836ac285098897b992b5f23b936706c5e6d9ad156453a24eb5acf09b670daa153ef7dde7cb88c3af40fa3928836c730781e5ef22c596a7d7495f8bac8c952e33e403c031162e3cfe24dee22f66d9e873dcf26dbbc342ae9e66b67cfb37caeee01e9353aa11036a6ee569d6986437ccd29f080f2886e11e69b5b7abd0913250e394b17e9ce58eb6c72ca3fbaba40d87ec722fbdb035fa5f01de6c0173c257bd06ed64294db95786d0a0953ab04fe38189fb705f9cf816d92e88dd642bf1c19e0fd2b53c06b6e74352ea467cee8c422171e5d2dd34f9bcf4b75b11bd4146f659c7eaaa9f0f04c55616b1bea7ae84b0d16bd65e1fc05d7418660110f5fdea8058f6de4e3ab0ca7925e19e06a4385eb7f941761b0d884fc7ad011fe2f2a705e7016455a6f531318c1a7e59255cd443dfffb96f1190d4f53b1b5160909e87beb1bed8f8bcb9d960859cb38923b6ca5baf4c9c66488eeb775aca1e6920e0e53e68f61c6f976cf27fec124ad6099961cdaf48b106c9f016feac2230e59d9f08ad9310f6613ff8c2b84ae609eadeefe2c2675bc2f49b9f9db2b88c4e7a0cc53932bb762f76bd439231267fb96aaa9f956160ee95dbc682c8ba066e42d07739c0f5678f8f571837a912084522017591c2d32167f4615380de3e1e31d290e5e3e51f1fc3ddac58bb518478a00b5548adb13f3230a52e49832968893ec7625fabf0d86c7ae31c86b6d0affff83d0e2adbaace1caf70ebb072ef5ecefce8edec792847d41c6750a8126fba24b581d52403f4a8664e1f409034794875d2ffda9435d695e11132f0c9b676cd703f9d523d0dd8f4987478d4f2b30f4f062ad2d05fc742e91bc20ca3ee8a2d126aff592c7de19edb3b884550ddd6f99b0d0a6b2b785617b46c0995bc112176dbae9a5b7f0bec678e84d6f44c5b5a857e5309fcd07a800e744633f30ce36962cacc88bcd5999388ef17f11f569e70a8e79f8947897de692c99e2145c2c1cf364092bd8b257b792a0bbe14712ac127287ca814b320298e8cfd9c5cf1a75259da8628c02e355ca636289fc852926b87eb859631a85b2882e5566a6b61833f1136e4b7190fcde223958fe2942a544499da4a91698e041fca831d2dd6fb1e2fff33a1675194b7b5f72d88b0016139f564dd13c042cee1b96c50991026293fcb2ab7075fef27ba7e2ae85a31965f0926f0dd274b46cebb9c72331438e252b0bb9aab5cc87b2f9de5c5ad4f8cdb8d8c7e46c0cf5bfdf75970e313d4d092dd37a657f8e9c97d142991cf63adea585189a66d705318eeb185dec8da22febe349d356a84f68b7e3d262b50f4bd729614ca466dd3fba9a165058d286f46eaefbea90fda39cb3606145c5a5147c74d1c3f15054fd30e369e79a2270bba88beace325dcaffeaff19b0092c7d84413a27238338b1a57d6e96e5724517aa3309f7e0bd746f1baece4c769921af0d3e89a2169ec81d64699e8986553b57b1285b7ec9e89f3a957c83811b722fa9edc905db038078f8c8aa05ba40df7fb19efd02e7e04018bdfbc8c1501530b0fec475112a70f4e5c430d0a336b1ab6d7cb5922592cbca608b799d17fa220a31402e1fc74e18f55879103ae9a3f83c7bccb459368d82fea651fe17eeb08f510077b85e440f3ee727bd5d45e1c0167aa6ea6e42b455a5e39e26abeab166e0903ab21e1e5ff704fcef2d1bf0202d64cc6b5023ed038dbd1fc4540394049fde2e2c79dfce8f78037d1b42dcb8295e8bc8359885c6b88831199574b5825ccdd0d0a162496dfd6724930e3889001c61a5f3e4921df45e57e79a57ae0302543e73509088ab59095b6f6c16c37a267b7dc635b80d6af012462e61505a89aa880e22d651fc0a2190f0bad73d48a0c083b221a999040a299127cf1eaa4762fe8c504efa8c29004812abb96cc834676e93b33af);

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


INSERT INTO `patient_data` (`PESEL`, `first_name`, `last_name`) VALUES('44051401458', 'Jan', 'Kowalski');

-- --------------------------------------------------------

--
-- Struktura tabeli dla  `session`
--

CREATE TABLE IF NOT EXISTS `session` (
  `session_id` int(11) NOT NULL,
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
  `user_id` int(11) NOT NULL,
  `login` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  `password` varchar(45) COLLATE utf8_polish_ci NOT NULL,
  `privileges` int(11) DEFAULT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_polish_ci;

INSERT INTO `users` (`login`, `password`, `privileges`) VALUES('test','pass123','1');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
