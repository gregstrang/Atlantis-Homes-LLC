<?php
// $Id: default.settings.php,v 1.8.2.1 2008/08/13 06:52:36 dries Exp $

$_REAL_BASE_DIR = realpath(dirname(__FILE__)); // filesystem path of this file's directory (config.php)
$one_up = preg_replace("/\/[^\/]*$/i", "", $_REAL_BASE_DIR);

if (file_exists($one_up . "/settings.php"))
        require_once($one_up . "/settings.php");
		
$db_prefix = array(
	'menu_router'   => 'design_',
	'system'     => 'design_',
	'variable'  => 'design_',
	'cache'      => 'design_',
	'cache_block'   => 'design_',
	'cache_content'      => 'design_',
	'cache_filter'   => 'design_',
	'cache_form'      => 'design_',
	'cache_location'   => 'design_',
	'cache_menu'   => 'design_',
	'cache_page'   => 'design_',
	'cache_views'   => 'design_'
);
