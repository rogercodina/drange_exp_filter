<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_query_alter().
 *
 * Unica manera de modificar la query si el filtre estar exposat.
 */
function drange_exp_filter_views_query_alter(&$view, &$query) {

  $fname = drange_exp_filter_get_filter_field_name($view);

  /*
   * IMPORTANTE: Esta funcion NO se ejecuta para las vistas de Search API.
   */
  if (!empty($view->exposed_raw_input['fulld_yearmonth'])) {
    // We receive something like YYYY-MM.
    $fulld_yearmonth = $view->exposed_raw_input['fulld_yearmonth'];
    $params = explode("-", $fulld_yearmonth);
    $month_days = cal_days_in_month(CAL_GREGORIAN, $params[1], $params[0]);

    $ini = date('Ymd', strtotime($fulld_yearmonth . '-01'));
    $fin = date('Ymd', strtotime($fulld_yearmonth . '-' . $month_days));

    if (TRUE) {

    /*
	   *              INI            FIN
	   *               |--------------|
	   *
	   *
	   *  CASE 1          |--------|
	   *
	   *  CASE 2    |----------------------|
	   *
	   *  CASE 3                   |--------|
	   *
	   *  CASE 4    |--------|
	   *
	   */

      $case_1 = "(DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') >= :ini AND DATE_FORMAT(field_data_{$fname}.{$fname}_value2, '%Y%m%d') <= :fin)";
      $case_2 = "(DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') < :ini AND DATE_FORMAT(field_data_{$fname}.{$fname}_value2, '%Y%m%d') > :fin)";
      $case_3 = "(DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') > :ini AND DATE_FORMAT(field_data_{$fname}.{$fname}_value2, '%Y%m%d') > :fin) AND DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') <= :fin";
      $case_4 = "(DATE_FORMAT(field_data_{$fname}.{$fname}_value, '%Y%m%d') < :ini AND DATE_FORMAT(field_data_{$fname}.{$fname}_value2, '%Y%m%d') < :fin AND DATE_FORMAT(field_data_{$fname}.{$fname}_value2, '%Y%m%d') >= :ini)";

      $query->add_where_expression(1, '(' . $case_1 . ' OR ' . $case_2 . ' OR ' . $case_3 . ' OR '. $case_4 . ')', array(':ini' => $ini, ':fin' => $fin));
      $query->add_table("field_data_{$fname}");

      //dpm($query);
    }
  }


}

/**
 * Implements hook_search_api_views_query_alter().
 *
 * Unica manera de modificar la query si el filtre estar exposat.
 */
function drange_exp_filter_search_api_views_query_alter(&$view, &$query) {

  $fname = drange_exp_filter_get_filter_field_name($view);

  /*
   * IMPORTANTE: Esta funcion SOLO se ejecuta para las vistas de Search API.
   */
  if (!empty($view->exposed_raw_input['fulld_yearmonth'])) {
    // We receive something like YYYY-MM.
    $fulld_yearmonth = $view->exposed_raw_input['fulld_yearmonth'];
    $params = explode("-", $fulld_yearmonth);
    $month_days = cal_days_in_month(CAL_GREGORIAN, $params[1], $params[0]);

    $ini = date('Ymd', strtotime($fulld_yearmonth . '-01'));
    $fin = date('Ymd', strtotime($fulld_yearmonth . '-' . $month_days));

    if (TRUE) {

      /*
	   *              INI            FIN
	   *               |--------------|
	   *
	   *
	   *  CASE 1          |--------|
	   *
	   *  CASE 2    |----------------------|
	   *
	   *  CASE 3                   |--------|
	   *
	   *  CASE 4    |--------|
	   *
	   */

    /**
	   * NOTE: Search API stores dates as timestamps
	   *
	   * http://enzolutions.com/articles/2015/06/10/how-to-customize-a-search-api-filters-in-drupal-7/
	   * https://github.com/markaspot/mark-a-spot/blob/master/profiles/markaspot/modules/mark_a_spot/modules/markaspot_radar/markaspot_radar.module
	   */
      $query->addField("{$fname}:value");
      $query->addField("{$fname}:value2");

      // CASE 1
      $subfilter1 = $query->createFilter('AND');
      $subfilter1->condition("{$fname}:value", $ini , '>=')
                 ->condition("{$fname}:value2", $fin, '<=');

      // CASE 2
      $subfilter2 = $query->createFilter('AND');
      $subfilter2->condition("{$fname}:value", $ini , '<')
                 ->condition("{$fname}:value2", $fin, '>');

      // CASE 3
      $subfilter3 = $query->createFilter('AND');
      $subfilter3->condition("{$fname}:value", $ini , '>')
                 ->condition("{$fname}:value2", $fin, '>')
                 ->condition("{$fname}:value", $fin , '<=');

      // CASE 4
      $subfilter4 = $query->createFilter('AND');
      $subfilter4->condition("{$fname}:value", $ini , '<')
                 ->condition("{$fname}:value2", $fin, '<')
                 ->condition("{$fname}:value2", $ini , '>=');

      $main_filter = $query->createFilter('OR');
      $main_filter->filter($subfilter1);
      $main_filter->filter($subfilter2);
      $main_filter->filter($subfilter3);
      $main_filter->filter($subfilter4);

      $query->filter($main_filter);

      //dpm($query);
    }
  }
}

/**
 * Get filter_field_name option.
 *
 * param $view object;
 *
 * @return string Field name.
 */
function drange_exp_filter_get_filter_field_name($view) {
  $field_name = 'field_date_from_to';

  foreach($view->display_handler->handlers['filter'] as $filter) {
    if (is_object($filter) && is_a($filter, 'drange_exp_filter_handler_filter_months_sapi')) {
      if (!empty($filter->options['filter_field_name'])) {
        $field_name = $filter->options['filter_field_name'];
      }
    }
  }

  return $field_name;
}
