<?php
// $Id: nat.views.inc,v 1.2 2008/11/10 19:44:33 anantagati Exp $

function nat_views_data() {
  $data = array();

  $data['nat'] = array(
    'table' => array(
      'group' => t('NAT'), 
    ),
  );
  $data['nat']['table']['join']['node'] = array(
    'left_table' => 'term_node',
    'left_field' => 'tid',
    'field' => 'tid',
  );
  $data['nat']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of the NAT node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}