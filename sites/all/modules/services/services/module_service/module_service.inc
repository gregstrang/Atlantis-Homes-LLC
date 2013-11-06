<?php
// $Id: module_service.inc,v 1.1.2.6 2008/09/06 04:13:08 marcingy Exp $
/**
 * @author Services Dev Team
 * @file
 *  Link general module functionalities to services module.
 */

function inclind_install_module($module) {
	include_once('includes/install.inc');
	drupal_install_modules(array($module));

	cache_clear_all();
	return "Installed $module";
	//return drupal_install_modules(array($module));
}


function module_service_install($module, $version) {
	$incoming_directory = drupal_get_path('module', 'module_service') . '/incoming';


	if (!isset($module ))
		return services_error(t('module not specified'));

	if (!isset($version ))
		return services_error(t('module version not specified'));

	if (!is_dir($incoming_directory . '/modules/' . $module))
		return services_error(t('module directory not found'));

	$tarball = $incoming_directory . '/modules/' . $module . '/' . $module . '-' . $version . '.tar.gz';
	if (!file_exists($tarball))
		return services_error(t('module version tarball not found'));
		
	$module_directory = $incoming_directory . '/../../../../' . $module;
	exec('rm -rf ' . $module_directory);
	mkdir($module_directory);

	exec('chmod -R 777 ' . $module_directory);
	exec('tar -xzvf ' . $tarball . ' -C ' . $module_directory . '/../ > /tmp/test123');
	exec('chmod -R 777 ' . $module_directory);

	$output .= inclind_install_module($module);
	return "Installing $module ($module_directory): " . $output;
}


function module_service_activate($module) {
	if (!isset($module ))
		return services_error(t('module not specified'));

	$result = module_enable(array($module));
	cache_clear_all();
	return "Enabled $module" . $result;
}

function module_service_deactivate($module) {
	if (!isset($module ))
		return services_error(t('module not specified'));

	$result = module_disable(array($module));
	cache_clear_all();
	return "Disabled $module" . $result;
}


function module_service_save($filename, $file) {
	$incoming_directory = drupal_get_path('module', 'module_service') . '/incoming';

	$module = $filename;
	$module = preg_replace('/-.*/', '', $filename);

	$module_dir = $incoming_directory . "/modules/" . $module;

	if (!file_exists($module_dir))
		mkdir($module_dir, 0775);

	$outfile = fopen($incoming_directory . "/modules/" . $module . '/' . $filename, "wb");
	fwrite($outfile, base64_decode($file));
	fclose($outfile);

	return "Received";
}


function module_service_list() {
	$output = '';

	$result = array();

	$modules = db_query("SELECT * FROM {system} WHERE type = 'module'");

	while ($module = db_fetch_array($modules)) {
		$module_info = unserialize($module['info']);

		$filename = $module['filename'];
		$directory = preg_replace("/\/[^\/]*$/", "", $filename);
		$hash = md5_dir($directory);
		$vital_info = array("project" => $module_info['project'], "name" => $module_info['name'], "package" => $module_info['package'], "version" => $module_info['version'], "status" => $module['status'], "hash" => $hash);
		foreach($vital_info AS $field => $value)
			$result[$module_info['name']][$field] = $value;

	}

	return $result;
}


/**
 * Returns a specified module.
 *
 * @param $nid
 *   Number. The module ID.
 * @param $fields
 *   Array (optinonal). The module fields needed. If its empty,
 *   all fields will be returned
 * @return
 *   Object. The module object with all wanted fields.
 */
function module_service_get($nid, $fields = array()) {
  if (!$module = services_module_load(module_load(array('nid' => $nid)), $fields)) {
    return services_error(t('Could not find the module.'));
  }

  return $module;
}


/**
 * Check if the user has the permission to get the
 * module's data thru services.
 *
 * @param $nid
 *   Number. The module ID.
 * @return
 *   Boolean. TRUE if the user has the permission to get the
 *   module's data thru services.
 */
function module_service_get_access($nid) {
  global $user;
  $module = module_load($nid);
  return ($module->uid == $user->uid and user_access('load own module data')) or
    ($module->uid != $user->uid and user_access('load any module data'));
}

/**
 * Save a module. It creates a new one, in case the 'nid' field
 * is missing.
 *
 * @param $edit
 *   Array. The module fields' values, just like created on
 *   module edit form.
 * @return
 *   Number. The module ID.
 */
function _olD_module_service_save($edit) {
  // Load the required includes for drupal_execute
  module_load_include('inc', 'module', 'module.pages');
  $nid = NULL;

  if ($edit['nid']) {
    $module = module_load($edit['nid']);
    if ($module->nid) {
      // Setup form_state.
      $form_state = array();
      $form_state['values'] = (array) $edit;
      $form_state['values']['op'] = t('Save');

      // Later on in the chain module_form seems to expect a copy of
      // the old module object.
      $form_state['module'] = (array) $module;

      $ret = drupal_execute($module->type .'_module_form', $form_state, (object)$module);

      // If the module is immediately reloaded after update, it will
      // load the OLD cached version.
      module_load(0, NULL, TRUE);

      // Set $nid, so it can be returned
      $nid = $module->nid;
    }
    else {
      return services_error(t('module not found'));
    }
  }
  else {
    // Setup form_state
    $form_state = array();
    $form_state['values'] = (array) $edit;
    $form_state['values']['op'] = t('Save');

    $ret = drupal_execute($edit['type'] .'_module_form', $form_state, (object)$edit);

    // Fetch $nid out of $form_state
    $nid = $form_state['nid'];
  }
  if ($errors = form_get_errors()) {
    return services_error(implode("\n", $errors));
  }

  watchdog('content', '@type: updated %title.',
    array('@type' => t($module->type), '%title' => $module->title),
    WATCHDOG_NOTICE, l(t('view'), 'module/'. $module->nid));
  return $nid;
}

/**
 * Check if the user has the permission to save a module.
 *
 * @param $module
 *   Object. The module object.
 * @return
 *   Boolean. TRUE if the user has the permission to save a module.
 */
function module_service_save_access($module) {
  if (isset($module['nid'])) {
    return module_access('update', $module);
  }
  return module_access('create', $module['type']);
}

/**
 * Check if the user has the permission to delete a module.
 *
 * @param $nid
 *   Number. The module ID.
 * @return
 *   Boolean. TRUE if the user has the permission to delete a module.
 */
function module_service_delete_access($nid) {
  $module = module_load($nid);
  return module_access('delete', $module);
}


function md5_dir($dir) {
    if (!is_dir($dir))
    {
        return false;
    }

    $filemd5s = array();
    $d = dir($dir);

    while (false !== ($entry = $d->read()))
    {
        if ($entry != '.' && $entry != '..')      
        {
             if (is_dir($dir.'/'.$entry))
             {
                 $filemd5s[] = MD5_DIR($dir.'/'.$entry);    
             }
             else
             {
                 $filemd5s[] = md5_file($dir.'/'.$entry);
             }
         }
    }
    $d->close();            
    return md5(implode('', $filemd5s));
}

