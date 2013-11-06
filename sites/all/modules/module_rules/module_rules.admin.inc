<?php
// $Id: module_rules.admin.inc,v 1.3.2.10 2008/06/01 12:51:51 hass Exp $

/**
 * @file
 * Administrative page callbacks for the module_rules module.
 */

function module_rules_admin_settings_form(&$form_state, $module_rule = NULL) {

  if ($module_rule) {
    $edit = (array)$module_rule;
  }
  else {
    $edit = NULL;
  }

  $form['module_rules_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Module settings')
  );

	
	$modules = module_list(true, false, true);
	$module_list = array("" => "");
	sort($modules);

	foreach ($modules AS $module) {
		$module_list["$module"] = $module;
	}

  $form['module_rules_settings']['name'] = array(
//    '#type' => 'textfield',
'#type' => 'select',
'#options' => $module_list,
'#required' => true,
    '#title' => t('Name'),
    '#default_value' => $edit['name'],
//    '#size' => 40,
    '#maxlength' => 64,
    '#description' => t('Give the name of the module.'),

  );
  $form['module_rules_settings']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $edit['status'],
    '#description' => t('Enable or disable this module rule.')
  );
/*
  $form['module_rules_settings']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Select theme'),
    '#default_value' => $edit['theme'],
    '#options' => _module_rules_theme_select(),
    '#description' => t('Select the theme you want to use for this section. Disabled themes are not used until they are enabled on <a href="@url">themes</a> page.', array('@url' => url('admin/build/themes')))
  );
*/
  $form['module_rules_settings']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'] ? $edit['weight'] : 0
  );

  // Role-based visibility settings.
  $default_role_options = array();
  $result = db_query('SELECT rid FROM {module_rules_roles} WHERE sid = %d', $edit['sid'] ? $edit['sid'] : 0);
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
    drupal_set_title(t('Edit module %name', array('%name' => $module_rule->name)));
    $form['sid'] = array(
      '#type' => 'hidden',
      '#value' => $edit['sid']
    );
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('module_rules_admin_settings_form_save_function')
    );
  }
  else {
    // We are adding a new module.
    drupal_set_title(t('Add module'));
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add module')
    );
  }

  return $form;
}

function module_rules_admin_settings_form_save_function($form, &$form_state) {
  db_query("UPDATE {module_rules_data} SET name = '%s', status = %d, visibility = %d, theme = '%s', path = '%s', weight = %d WHERE sid = %d", $form_state['values']['name'], $form_state['values']['status'], $form_state['values']['visibility'], $form_state['values']['theme'], $form_state['values']['path'], $form_state['values']['weight'], $form_state['values']['sid']);
  db_query("DELETE FROM {module_rules_roles} WHERE sid = %d", $form_state['values']['sid']);
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    db_query("INSERT INTO {module_rules_roles} (rid, sid) VALUES (%d, %d)", $rid, $form_state['values']['sid']);
  }
  drupal_set_message(t('The module_rules configuration has been saved.'));
  $form_state['redirect'] = 'admin/build/module_rules';
}

function module_rules_admin_settings_form_submit($form, &$form_state) {
  db_query("INSERT INTO {module_rules_data} (name, status, visibility, path, theme, weight) VALUES ('%s', %d, %d, '%s', '%s', %d)", $form_state['values']['name'], $form_state['values']['status'], $form_state['values']['visibility'], $form_state['values']['path'], $form_state['values']['theme'], $form_state['values']['weight']);
  $sid = db_last_insert_id('module_rules_data', 'sid');
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    db_query("INSERT INTO {module_rules_roles} (rid, sid) VALUES (%d, %d)", $rid, $sid);
  }
  drupal_set_message(t('The module_rules configuration has been saved.'));
  $form_state['redirect'] = 'admin/build/module_rules';
}

function module_rules_delete_form(&$form_state, $module_rule) {
  $form['name'] = array('#type' => 'hidden', '#value' => $module_rule->name);
  $form['sid'] = array('#type' => 'hidden', '#value' => $module_rule->sid);

  return confirm_form(
    $form,
    t('Delete section %name', array('%name' => $module_rule->name)),
    'admin/build/module_rules',
    '<p>'. t('Are you sure you want to delete the section %name?', array('%name' => $module_rule->name)) .'</p>',
    t('Delete'),
    t('Cancel')
  );
}

function module_rules_delete_form_submit($form, &$form_state) {
  db_query('DELETE FROM {module_rules_data} WHERE sid = %d', $form_state['values']['sid']);
  drupal_set_message(t('The section %name has been deleted.', array('%name' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/build/module_rules';
  return;
}

/**
 * Build the form for ordering module_rules.
 */
function module_rules_admin_display_form(&$form_state) {
  $module_rules = _module_rules_load();

  // Create tree for submitted data.
  $form['module_rules'] = array('#tree' => TRUE);

  foreach ($module_rules as $module_rule) {
    $form['module_rules'][$module_rule->sid]['sid'] = array(
      '#type' => 'hidden',
      '#value' => $module_rule->sid
    );
    $form['module_rules'][$module_rule->sid]['name'] = array(
      '#value' => $module_rule->name
    );
    $form['module_rules'][$module_rule->sid]['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => $module_rule->status
    );
    $form['module_rules'][$module_rule->sid]['theme'] = array(
      '#value' => $module_rule->theme
    );
    $form['module_rules'][$module_rule->sid]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $module_rule->weight
    );
    $form['module_rules'][$module_rule->sid]['edit'] = array(
      '#value' => l(t('Edit'), 'admin/build/module_rules/edit/'. $module_rule->sid)
    );
    $form['module_rules'][$module_rule->sid]['delete'] = array(
      '#value' => l(t('Delete'), 'admin/build/module_rules/delete/'. $module_rule->sid)
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
function theme_module_rules_admin_display_form($form) {
  drupal_add_tabledrag('module_rules-order', 'order', 'sibling', 'module_rules-order-weight');

  $header = array(
    array('data' => t('Section')),
    array('data' => t('Enabled')),
//    array('data' => t('Theme')),
    array('data' => t('Weight')),
    array('data' => t('Operations'), 'colspan' => '2')
  );

  $rows = array();
  foreach (element_children($form['module_rules']) as $key => $id) {
    // Don't take form control structures.
    if (is_array($form['module_rules'][$id]['name'])) {
      $form['module_rules'][$id]['weight']['#attributes']['class'] = 'module_rules-order-weight';
      $rows[] = array(
        'data' => array(
          drupal_render($form['module_rules'][$id]['name']),
          drupal_render($form['module_rules'][$id]['status']),
//          drupal_render($form['module_rules'][$id]['theme']),
          drupal_render($form['module_rules'][$id]['weight']),
          drupal_render($form['module_rules'][$id]['edit']),
          drupal_render($form['module_rules'][$id]['delete'])
        ),
        'class' => 'draggable',
      );
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No module_rules available.'), 'colspan' => '6'));
  }

  $output = theme('table', $header, $rows, array('id' => 'module_rules-order'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Process module_rules order configuration form submission.
 */
function module_rules_admin_display_form_submit($form, &$form_state) {
  foreach ($form_state['values']['module_rules'] as $module_rule) {
    db_query("UPDATE {module_rules_data} SET status = %d, weight = %d WHERE sid = %d", $module_rule['status'], $module_rule['weight'], $module_rule['sid']);
  }
  drupal_set_message(t('The module_rules configuration has been saved.'));
}

/**
 * Load all module_rules.
 */
function _module_rules_load() {
  $res = db_query('SELECT * FROM {module_rules_data} ORDER BY weight');
  $module_rules = array();
  while ($row = db_fetch_object($res)) {
    $module_rules[] = $row;
  }
  return $module_rules;
}

/**
 * Loads the options for the themes select form element.
 */
function _module_rules_theme_select() {
  $options = array();

  foreach (list_themes() as $key => $theme) {
    $options[$theme->name] = t('@name (@status)', array('@name' => $theme->name, '@status' => ($theme->status ? t('Enabled') : t('Disabled'))));
  }
  return $options;
}
