<?php
// $Id: resource_rules.admin.inc,v 1.3.2.10 2008/06/01 12:51:51 hass Exp $

/**
 * @file
 * Administrative page callbacks for the resource_rules module.
 */

function resource_rules_admin_settings_form(&$form_state, $resource_rule = NULL) {
  if ($resource_rule) {
    $edit = (array)$resource_rule;
  }
  else {
    $edit = NULL;
  }

  $form['resource_rules_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Resource Rules settings')
  );

	
/*
	$modules = module_list(true, false, true);
	$module_list = array("" => "");
	sort($modules);

	foreach ($modules AS $module) {
		$module_list["$module"] = $module;
	}
*/

$current_theme = variable_get('theme_default','none');

$themes = list_themes();
$theme_object = $themes[$current_theme];

// print all the object fields
$theme_path = preg_replace('/[^\/]*$/', '', $theme_object->filename);

  $form['resource_rules_settings']['name'] = array(
    '#type' => 'textfield',
/*
'#type' => 'select',
'#options' => $module_list,
*/
'#required' => true,
    '#title' => t('Name'),
    '#default_value' => $edit['name'],
    '#size' => 40,
    '#field_prefix' => $theme_path,
    '#maxlength' => 64,
    '#description' => t('Enter the path, relative to the current theme directory, to the resource file'),

  );
  $form['resource_rules_settings']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($edit['status']) ? $edit['status'] : 1,
    '#description' => t('Enable or disable this module rule.')
  );
/*
  $form['resource_rules_settings']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Select theme'),
    '#default_value' => $edit['theme'],
    '#options' => _resource_rules_theme_select(),
    '#description' => t('Select the theme you want to use for this section. Disabled themes are not used until they are enabled on <a href="@url">themes</a> page.', array('@url' => url('admin/build/themes')))
  );
*/
  $form['resource_rules_settings']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'] ? $edit['weight'] : 0
  );

  // Role-based visibility settings.
  $default_role_options = array();
  $result = db_query('SELECT rid FROM {resource_rules_roles} WHERE sid = %d', $edit['sid'] ? $edit['sid'] : 0);
  while ($role = db_fetch_object($result)) {
    $default_role_options[] = $role->rid;
  }
  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific visibility settings'),
    '#collapsible' => TRUE,
  );
  $form['role_vis_settings']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show module for specific roles'),
    '#default_value' => $default_role_options,
    '#options' => $role_options,
    '#description' => t('Show this module only for the selected role(s). If you select no roles, the section will be visible to all users. If a user has any of the roles checked, the section will be visible for this user.'),
  );

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
  );

  $access = user_access('use PHP for block visibility');
  $visibility = $edit['visibility'] ? $edit['visibility'] : 0;

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $edit['path']);
  }
  else {
    $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Activate module on the specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['path'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $edit['path'],
      '#description' => $description,
    );
  }

  if (!empty($edit['sid'])) {
    // We are updating a module rule.
    drupal_set_title(t('Edit resource rule %name', array('%name' => $resource_rule->name)));
    $form['sid'] = array(
      '#type' => 'hidden',
      '#value' => $edit['sid']
    );
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('resource_rules_admin_settings_form_save_function')
    );
  }
  else {
    // We are adding a new resource rule.
    drupal_set_title(t('Add resource rule'));
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add resource rule')
    );
  }

  return $form;
}

function resource_rules_admin_settings_form_save_function($form, &$form_state) {
  db_query("UPDATE {resource_rules_data} SET name = '%s', status = %d, visibility = %d, theme = '%s', path = '%s', weight = %d WHERE sid = %d", $form_state['values']['name'], $form_state['values']['status'], $form_state['values']['visibility'], $form_state['values']['theme'], $form_state['values']['path'], $form_state['values']['weight'], $form_state['values']['sid']);
  db_query("DELETE FROM {resource_rules_roles} WHERE sid = %d", $form_state['values']['sid']);
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    db_query("INSERT INTO {resource_rules_roles} (rid, sid) VALUES (%d, %d)", $rid, $form_state['values']['sid']);
  }
  drupal_set_message(t('The resource_rules configuration has been saved.'));
  $form_state['redirect'] = 'admin/build/resource_rules';
}

function resource_rules_admin_settings_form_submit($form, &$form_state) {
  db_query("INSERT INTO {resource_rules_data} (name, status, visibility, path, theme, weight) VALUES ('%s', %d, %d, '%s', '%s', %d)", $form_state['values']['name'], $form_state['values']['status'], $form_state['values']['visibility'], $form_state['values']['path'], $form_state['values']['theme'], $form_state['values']['weight']);
  $sid = db_last_insert_id('resource_rules_data', 'sid');
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    db_query("INSERT INTO {resource_rules_roles} (rid, sid) VALUES (%d, %d)", $rid, $sid);
  }
  drupal_set_message(t('The resource_rules configuration has been saved.'));
  $form_state['redirect'] = 'admin/build/resource_rules';
}

function resource_rules_delete_form(&$form_state, $resource_rule) {
  $form['name'] = array('#type' => 'hidden', '#value' => $resource_rule->name);
  $form['sid'] = array('#type' => 'hidden', '#value' => $resource_rule->sid);

  return confirm_form(
    $form,
    t('Delete section %name', array('%name' => $resource_rule->name)),
    'admin/build/resource_rules',
    '<p>'. t('Are you sure you want to delete the section %name?', array('%name' => $resource_rule->name)) .'</p>',
    t('Delete'),
    t('Cancel')
  );
}

function resource_rules_delete_form_submit($form, &$form_state) {
  db_query('DELETE FROM {resource_rules_data} WHERE sid = %d', $form_state['values']['sid']);
  drupal_set_message(t('The section %name has been deleted.', array('%name' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/build/resource_rules';
  return;
}

/**
 * Build the form for ordering resource_rules.
 */
function resource_rules_admin_display_form(&$form_state) {
  $resource_rules = _resource_rules_load();

  // Create tree for submitted data.
  $form['resource_rules'] = array('#tree' => TRUE);

  foreach ($resource_rules as $resource_rule) {
    $form['resource_rules'][$resource_rule->sid]['sid'] = array(
      '#type' => 'hidden',
      '#value' => $resource_rule->sid
    );
    $form['resource_rules'][$resource_rule->sid]['name'] = array(
      '#value' => $resource_rule->name
    );
    $form['resource_rules'][$resource_rule->sid]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $resource_rule->status
    );
    $form['resource_rules'][$resource_rule->sid]['theme'] = array(
      '#value' => $resource_rule->theme
    );
    $form['resource_rules'][$resource_rule->sid]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $resource_rule->weight
    );
    $form['resource_rules'][$resource_rule->sid]['edit'] = array(
      '#value' => l(t('Edit'), 'admin/build/resource_rules/edit/'. $resource_rule->sid)
    );
    $form['resource_rules'][$resource_rule->sid]['delete'] = array(
      '#value' => l(t('Delete'), 'admin/build/resource_rules/delete/'. $resource_rule->sid)
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
  );

  return $form;
}

/**
 * Theme section order configuration form.
 */
function theme_resource_rules_admin_display_form($form) {
  drupal_add_tabledrag('resource_rules-order', 'order', 'sibling', 'resource_rules-order-weight');

  $header = array(
    array('data' => t('Resource file')),
    array('data' => t('Enabled')),
//    array('data' => t('Theme')),
    array('data' => t('Weight')),
    array('data' => t('Operations'), 'colspan' => '2')
  );

  $rows = array();
  foreach (element_children($form['resource_rules']) as $key => $id) {
    // Don't take form control structures.
    if (is_array($form['resource_rules'][$id]['name'])) {
      $form['resource_rules'][$id]['weight']['#attributes']['class'] = 'resource_rules-order-weight';
      $rows[] = array(
        'data' => array(
          drupal_render($form['resource_rules'][$id]['name']),
          drupal_render($form['resource_rules'][$id]['status']),
//          drupal_render($form['resource_rules'][$id]['theme']),
          drupal_render($form['resource_rules'][$id]['weight']),
          drupal_render($form['resource_rules'][$id]['edit']),
          drupal_render($form['resource_rules'][$id]['delete'])
        ),
        'class' => 'draggable',
      );
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No resource rules available.'), 'colspan' => '6'));
  }

  $output = theme('table', $header, $rows, array('id' => 'resource_rules-order'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Process resource_rules order configuration form submission.
 */
function resource_rules_admin_display_form_submit($form, &$form_state) {
  foreach ($form_state['values']['resource_rules'] as $resource_rule) {
    db_query("UPDATE {resource_rules_data} SET status = %d, weight = %d WHERE sid = %d", $resource_rule['status'], $resource_rule['weight'], $resource_rule['sid']);
  }
  drupal_set_message(t('The resource Rules configuration has been saved.'));
}

/**
 * Load all resource_rules.
 */
function _resource_rules_load() {
  $res = db_query('SELECT * FROM {resource_rules_data} ORDER BY weight');
  $resource_rules = array();
  while ($row = db_fetch_object($res)) {
    $resource_rules[] = $row;
  }
  return $resource_rules;
}

/**
 * Loads the options for the themes select form element.
 */
function _resource_rules_theme_select() {
  $options = array();

  foreach (list_themes() as $key => $theme) {
    $options[$theme->name] = t('@name (@status)', array('@name' => $theme->name, '@status' => ($theme->status ? t('Enabled') : t('Disabled'))));
  }
  return $options;
}


