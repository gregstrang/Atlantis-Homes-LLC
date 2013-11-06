<?php
// $Id: node_access.inc,v 1.2 2009/01/29 22:12:05 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control based upon node path.
 */

/**
 * Implementation of specially named hook_ctools_arguments().
 */
function inclind_panels_node_path_ctools_access() {
$args['node_path'] = array(
'title' => t("Node access by path"),
'description' => t('Control access by node_access.'),
'callback' => 'inclind_panels_node_path_ctools_access_check',
'default' => array('type' => array()),
'settings form' => 'inclind_panels_node_path_ctools_access_settings',
'settings form submit' => 'inclind_panels_node_path_ctools_access_settings_submit',
'summary' => 'inclind_panels_node_path_ctools_acesss_summary',
'required context' => new ctools_context_required(t('Node'), 'node'),
);

return $args;
}

/**
 * Settings form for the 'by node_access' access plugin
 */
function inclind_panels_node_path_ctools_access_settings(&$form, &$form_state, $conf) {
	$form['settings']['page_vis_settings'] = array(
		'#type' => 'fieldset',
		'#title' => t('Page specific visibility settings'),
		'#collapsible' => TRUE,
	);

	$options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
	$description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

	$options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
	$description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));

	$form['settings']['page_vis_settings']['visibility'] = array(
		'#type' => 'radios', 
		'#title' => t('Show block on specific pages'),
		'#options' => $options,
		'#default_value' => $conf['page_vis_settings']['visibility'],
	);

	$form['settings']['page_vis_settings']['pages'] = array(
		'#type' => 'textarea',
		'#title' => t('Pages'),
		'#default_value' => $conf['page_vis_settings']['pages'],
		'#description' => $description,
	);

	return;

	$form['settings']['path'] = array(
		'#title' => t('node path'),
		'#type' => 'textfield',
		'#description' => t('Only nodes inside of the indicated path will be valid'),
		'#default_value' => $conf['path'],
	);
}

/**
 * Compress the node_accesss allowed to the minimum.
 */
function inclind_panels_node_path_ctools_access_settings_submit(&$form, &$form_state) {

$form_state['values']['settings']['path'] = $form_state['values']['settings']['path'];


}

/**
 * Check for access.
 */
function inclind_panels_node_path_ctools_access_check($conf, $context) {

      // Match path if necessary
      if ($conf['page_vis_settings']['pages']) {
        if ($conf['page_vis_settings']['visibility'] < 2) {
          $path = drupal_get_path_alias($_GET['q']);                 
          // Compare with the internal and path alias (if any).   
          $page_match = drupal_match_path($path, $conf['page_vis_settings']['pages']);          
          if ($path != $_GET['q']) {
            $page_match = $page_match || drupal_match_path($_GET['q'], $conf['page_vis_settings']['pages']);
          }
          // When $conf['page_vis_settings']['visibility'] has a value of 0, the block is displayed on
          // all pages except those listed in $conf['page_vis_settings']['pages']. When set to 1, it
          // is displayed only on those pages listed in $conf['page_vis_settings']['pages'].     
          $page_match = !($conf['page_vis_settings']['visibility'] xor $page_match);
        }
        else {
          $page_match = drupal_eval($conf['page_vis_settings']['pages']);
        }
      }
      else {
        $page_match = TRUE;
      }

	return $page_match;
}

/**
 * Provide a summary description based upon the checked node_accesss.
 */
function inclind_panels_node_path_ctools_acesss_summary($conf, $context) {
	if (!isset($conf['page_vis_settings']['visibility'])) {
		$conf['page_vis_settings']['visibility'] = "";
	}

	if (!isset($conf['page_vis_settings']['pages'])) {
		$conf['page_vis_settings']['pages'] = "";
	}


	$pages = "[empty]";
	if (strlen($conf['page_vis_settings']['pages'])) {
		$pages = $conf['page_vis_settings']['pages'];
		$pages = str_replace("<", "&lt;", $pages);
		$pages = str_replace(">", "&gt;", $pages);
	}

	$visibility = $conf['page_vis_settings']['visibility'];

	if ($visibility == "0")
		$visibility = "[not match]";
	if ($visibility == "1")
		$visibility = "[match]";
	elseif ($visibility == "2") 
		$visibility = "[php]";

	return t('@identifier must ' . $visibility . ' page(s) ' . $pages,array('@identifier' => $context->identifier));
}



function inclind_panels_match_path($path, $patterns) {
	static $regexps;

	$path = ltrim(getenv('request_uri'), '/');
	if (!isset($regexps[$patterns])) {
		$regexps[$patterns] = '/^('. preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/', '/(^|\|)\\\\<front\\\\>($|\|)/'), array('|', '.*', '\1'. preg_quote(variable_get('site_frontpage', 'node'), '/') .'\2'), preg_quote($patterns, '/')) .')$/';
	}

	return preg_match($regexps[$patterns], $path);
}
