/***
inclind_menu.js

Pete Stein
Inclind, Inc.
2009

A utility script for synchronizing the menu link title in a node add/edit page with the title of the node.
***/

var inclind_menu_lastPageTitle = '';

$(document).ready( function() {
	inclind_menu_monitor_title();
	if ($('#edit-menu-link-title').attr('value') == '') {
		$('#edit-menu-link-title').attr('value', $('#edit-title').attr('value'));
	}
	lastTitle = $('#edit-title').attr('value');
});

function inclind_menu_monitor_title() {
	$('#edit-title').change( function() {
		var inclind_menu_newPageTitle = $(this).attr('value');
		var inclind_menu_menuTitle = $('#edit-menu-link-title').attr('value');

		if (inclind_menu_menuTitle == "" || inclind_menu_menuTitle == inclind_menu_lastPageTitle)
			$('#edit-menu-link-title').attr('value', inclind_menu_newPageTitle);

		inclind_menu_lastPageTitle = inclind_menu_newPageTitle;
	});
}
