<?php

/**
 * @file i18n_media.admin.inc
 *  Module settings forms and callbacks for the i18n_media module.
 */

/**
 * fn_i18n Media settings form.
 **/
function i18n_media_admin($form, &$form_state) {
  $form = array();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media translation settings'),
    '#description' => '<p>' .
                        t('When media translation is enabled images, form inputs of type "image", and flash objects have their <code>src="path-to-media.jpg"</code> altered to match the current language, eg: <code>src="path-to-media_de.jpg"</code>.  If the file does not exist for the translated media, the original untranslated version is used instead.') .
                      '</p><br /><p>' .
                        t('<strong>Note that this module is intended to only effect custom theme and module media</strong>. That is to say, NOT files attached to nodes or taxonomy.  You should use Drupal\'s content translation system and modules provided by the <a href="http://drupal.org/project/i18n">i18n</a> project to translate nodes, taxonomy, CCK, views, panels, etc.') .
                      '</p>',
  );
  $form['settings']['i18n_media_enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Enable media translation?'),
    '#default_value' => variable_get('i18n_media_enabled', 0),
    '#options' => array(
      0 => t('Off'),
      1 => t('On'),
    ),
  );

  $form['tests'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media translation tests'),
    '#collapsible' => TRUE,
    '#collapsed' => (!isset($_GET['test']) || !$_GET['test']),
    '#description' => t('These media translation tests visually show several web media types being swapped for translated versions.  Currently the tests are only configured to work for the languages: German (de), Spanish (es), French (fr), Japanese (ja) and Simplified Chinese (zh-hans).  To run the tests just switch to one of these languages (hint: you must have one of these languages installed, of course...).'),
  );
  $form['tests']['test'] = array(
    '#markup' => theme('i18n_media_tests'),
  );

  $form = system_settings_form($form);
  // If the 'i18n_media_enabled' switch is changed the theme registry
  // needs a rebuild to remove or add our custom phptemplate_page() handler.
  array_unshift($form['#submit'], 'i18n_media_admin_submit');
  return $form;
}

/**
 * Submit handler for fn_i18n Media settings form.
 **/
function i18n_media_admin_submit($form, &$form_state) {
  drupal_theme_rebuild();
}
