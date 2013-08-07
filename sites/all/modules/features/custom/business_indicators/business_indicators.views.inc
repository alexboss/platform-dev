<?php


/**
 * Implements hook_views_plugins()
 */
function business_indicators_views_plugins() {
  $plugins = array(
    'style' => array(
      'chart' => array(
        'title' => t('Chart'),
        'help' => t('Displays data using FusionCharts library.'),
        'handler' => 'business_indicators_views_plugin_style_chart',
        'theme' => 'business_indicators_views_view_chart',
        'uses row plugin' => FALSE,
        'uses row class' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
  return $plugins;
}

/**
 * Preprocess function for theme('business_indicators_views_view_chart');
 */
function template_preprocess_business_indicators_views_view_chart(&$vars) {

  libraries_load('FusionCharts');
  
  $view = $vars['view'];
  $vars['classes'] = 'chart-container';
  
  $options  = $view->style_plugin->options;
  $renders = $view->style_plugin->render_fields($vars['rows']);
  
  $data = array();
  foreach ($renders as $row) {
    $data_row = array();
    if (isset($row[$options['data_labels']]) && isset($row[$options['data_values']])) {
      $data_row[1] = str_replace("'", "&apos;", $row[$options['data_labels']]);
      $data_row[2] = $row[$options['data_values']];        
      $data[] = $data_row;
    }
  }
  
  foreach (array('caption', 'number_prefix') as $key) {
    $options[$key] = str_replace("'", "&apos;", $options[$key]);
  }
  
  $xml = "<chart caption='" . $options['caption'] . "' numberPrefix='" . htmlspecialchars($options['number_prefix']) . "' formatNumberScale='0'>";
  foreach ($data as $data_row) {
    $xml .= "<set label='" . $data_row[1] . "' value='" . $data_row[2] . "' />";
  }
  $xml .= "</chart>";
  
  switch ($options['type']) {
    case 'column2d':
      $component = 'Column2D.swf';
      break;
    case 'pie2d':
      $component = 'Pie2D.swf';
      break;
    case 'column3d':
      $component = 'Column3D.swf';
      break;
    case 'pie3d':
      $component = 'Pie3D.swf';
      break;
  }
  
//  $component_url = base_path() . libraries_get_path('FusionCharts') . '/' . $component;
//  $vars['chart'] = renderChart($component_url, "", $xml, $view->name . '_' . $view->current_display, $options['width'], $options['height'], FALSE, TRUE);      

  // Use external SWF file.
  $path = 'http://europa.eu/wel/fusioncharts/';
  $charts_path = $path . 'v0321/';
  $vars['chart'] = renderChart($charts_path . $component, "", $xml, $view->name . '_' . $view->current_display, $options['width'], $options['height'], FALSE, TRUE);      
}

/**
 * Theme function: display views data as a chart.
 */
function theme_business_indicators_views_view_chart($vars) {
  return '<div class="' . $vars['classes'] . '">' . $vars['chart'] . '</div>';
}
