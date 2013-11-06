<?php
$_REAL_BASE_DIR = realpath(dirname(__FILE__)); // filesystem path of this file's directory (config.php)
$one_up = preg_replace("/\/[^\/]*$/i", "", $_REAL_BASE_DIR);

if (file_exists($one_up . "/settings.php"))
        require_once($one_up . "/settings.php");

$db_url = 'mysqli://atlantishomesllc.com:jf9B3RH6KcYr@10.1.1.21/atlantishomesllc_com';
$db_prefix = array();
