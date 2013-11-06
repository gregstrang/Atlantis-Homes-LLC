<?php
// $Id: views_handler_field_taxonomy.inc,v 1.2 2008/11/19 22:44:30 merlinofchaos Exp $

/**
 * Field handler to provide simple renderer that allows linking to a taxonomy
 * term.
 */
class views_handler_field_taxonomy_inclind extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   *
   * This constructer assumes the term_data table. If using another
   * table, we'll need to be more specific.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['vid'] = 'vid';
    $this->additional_fields['tid'] = 'tid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_taxonomy'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to taxonomy option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its taxonomy term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );
  }

  /**
   * Render whatever the data is as a link to the taxonomy.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_taxonomy']) && !empty($values->{$this->aliases['tid']}) && $data !== NULL && $data !== '') {
      $term = new stdClass();
      $term->tid = $values->{$this->aliases['tid']};
      $term->vid = $values->{$this->aliases['vid']};

      return l($data, inclind_taxonomy_term_path($term), array('html' => TRUE));
    }
    else {
      return $data;
    }
  }

  function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }
}

function inclind_taxonomy_term_path($term) {
	static $nat_types, $nat_vids;
	if (!isset($nat_types)) {
		$nat_types = _nat_variable_get();
		$nat_types = $nat_types['types'];
	}
	if (!isset($nat_vids)) {

		foreach ($nat_types AS $type => $vids) {
			if (array_key_exists($term->vid, $vids)) {
				$nat_vids[$term->vid] = $term->vid;
			}
		}
	}

	if (array_key_exists($term->vid, $nat_vids)) {
		$nids = array_keys(nat_get_nids(array($term->tid), FALSE));
		$nid = $nids[0]; // should only be one anyway;

		if (is_numeric($nid))
			return drupal_get_path_alias('node/' . $nid);
	}

	return taxonomy_term_path($term);
}
