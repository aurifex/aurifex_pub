<?php

/**
 * @file i18n_media_phptemplate_page.inc
 *  .
 **/

/**
 * phptemplate_render_template() override to post-process page HTML for
 * media / image translation.
 *
 * This code was largely lifted from theme_render_template().
 **/
function phptemplate_render_template($template_file, $variables) {
  extract($variables, EXTR_SKIP); // Extract the variables to a local namespace
  ob_start(); // Start output buffering
  include DRUPAL_ROOT . '/' . $template_file; // Include the template file
  $output = ob_get_clean(); // End buffering and capture contents

  if (preg_match('/page[A-Za-z0-9_-]*\.tpl\.php$/', $template_file)) {
    // Perform the image replacement
    $output = preg_replace_callback('/src="([^"]+)"/', 'i18n_media_preg_replace_callback', $output);
  }

  return $output; // Return the buffer contents
}

/**
 * PCRE callback for altering source paths to reference language specific
 * versions of media files.
 **/
function i18n_media_preg_replace_callback($matches) {
  global $language;

  // If all else fails, return the original string
  $ret = $matches[0];

  // Is some sort of protocol reference
  if (strpos($matches[1], '://') !== FALSE) {
    $parts = parse_url($matches[1]);
    // Remove any leading slashes
    $path = ltrim($parts['path'], '/');
  }
  // Something else, probably a relative path
  else {
    $relative_parts = explode('?', $matches[1]);
    $path = ltrim($relative_parts[0], '/');
  }

  // If the path beings with the file_directory_path() then skip it
  if (strpos($path, file_directory_path()) === 0) {
    return $matches[0];
  }

  $new_path =  preg_replace('/\.(png|jpg|jpeg|gif|swf)/i', '_' . $language->language . '\0', $path);

  // Only use the language specific path if the file exists
  if (file_exists($new_path)) {
    if (isset($parts)) {
      $parts['path'] = '/' . $new_path;

      $query_fragment = '';
      if ($parts['query']) {
        $query_fragment .= '?' . $parts['query'];
      }
      if ($parts['fragment']) {
        $query_fragment .= '#' . $parts['fragment'];
      }

      $ret = 'src="' . $parts['scheme'] . '://' . $parts['host'] . $parts['path'] . $query_fragment . '"';
    }
    else if (isset($relative_parts)) {
      $ret = 'src="/' . $new_path . (isset($relative_parts[1]) ? '?' . $relative_parts[1] : '') . '"';
    }
  }

  return $ret;
}
