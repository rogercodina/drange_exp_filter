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
  /*
   * IMPORTANTE: Esta funcion NO se ejecuta para las vistas de Search API.
   */
  if (!empty($view->exposed_raw_input['ymonth'])) {
    $ymonth = $view->exposed_raw_input['ymonth'];

    $hashIni = array(
      date('Ym', strtotime('first day of')) => date('Ymd', strtotime('first day of')),
      date('Ym', strtotime('first day of next month')) => date('Ymd', strtotime('first day of next month')),
      date('Ym', strtotime('first day of +2 months')) => date('Ymd', strtotime('first day of +2 months')),
    );
    
    $hashFin = array(
      date('Ym', strtotime('first day of')) => date('Ymd', strtotime('last day of')),
      date('Ym', strtotime('first day of next month')) => date('Ymd', strtotime('last day of next month')),
      date('Ym', strtotime('first day of +2 months')) => date('Ymd', strtotime('last day of +2 months')),
    );
    
    $ini = $hashIni[$ymonth];
    $fin = $hashFin[$ymonth];

    if (is_numeric($ymonth) && $ymonth != 0) {

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

      $case_1 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') >= :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') <= :fin)";
      $case_2 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') < :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') > :fin)";
      $case_3 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') > :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') > :fin) AND DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') <= :fin";
      $case_4 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') < :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') < :fin AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') >= :ini)";

      $query->add_where_expression(1, '(' . $case_1 . ' OR ' . $case_2 . ' OR ' . $case_3 . ' OR '. $case_4 . ')', array(':ini' => $ini, ':fin' => $fin));
      $query->add_table('field_data_field_date');
      
      //dpm($query);
    }
  }
  
  if (!empty($view->exposed_raw_input['years'])) {
    $years = $view->exposed_raw_input['years'];

    $yhash = array( -2 => date('Y') - 2,
      -1 => date('Y') - 1,
      0 => date('Y'),
      1 => date('Y') + 1,
      2 => date('Y') + 2,
    );

    $hashIni = array(
      $yhash[-2] => date('Ymd', strtotime('first day of January ' . $yhash[-2] )),
      $yhash[-1] => date('Ymd', strtotime('first day of January ' . $yhash[-1] )),
      $yhash[0] => date('Ymd', strtotime('first day of January ' . $yhash[0] )),
      $yhash[1] => date('Ymd', strtotime('first day of January ' . $yhash[1] )),
      $yhash[2] => date('Ymd', strtotime('first day of January ' . $yhash[2] )),
    );
    
    $hashFin = array(      
      $yhash[-2] => date('Ymd', strtotime('last day of December ' . $yhash[-2] )),
      $yhash[-1] => date('Ymd', strtotime('last day of December ' . $yhash[-1] )),
      $yhash[0] => date('Ymd', strtotime('last day of December ' . $yhash[0] )),
      $yhash[1] => date('Ymd', strtotime('last day of December ' . $yhash[1] )),
      $yhash[2] => date('Ymd', strtotime('last day of December ' . $yhash[2] )),
    );
    
    $ini = $hashIni[$years];
    $fin = $hashFin[$years];

    if (is_numeric($years) && $years != 0) {

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

      $case_1 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') >= :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') <= :fin)";
      $case_2 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') < :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') > :fin)";
      $case_3 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') > :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') > :fin) AND DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') <= :fin";
      $case_4 = "(DATE_FORMAT(field_data_field_date.field_date_value, '%Y%m%d') < :ini AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') < :fin AND DATE_FORMAT(field_data_field_date.field_date_value2, '%Y%m%d') >= :ini)";

      $query->add_where_expression(1, '(' . $case_1 . ' OR ' . $case_2 . ' OR ' . $case_3 . ' OR '. $case_4 . ')', array(':ini' => $ini, ':fin' => $fin));
      $query->add_table('field_data_field_date');
      
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
  /*
   * IMPORTANTE: Esta funcion SOLO se ejecuta para las vistas de Search API.
   */
  if (!empty($view->exposed_raw_input['ymonth'])) {
    $ymonth = $view->exposed_raw_input['ymonth'];

    $hashIni = array(
      date('Ym', strtotime('first day of')) => strtotime('first day of'),
      date('Ym', strtotime('first day of next month')) => strtotime('first day of next month'),
      date('Ym', strtotime('first day of +2 months')) => strtotime('first day of +2 months'),
    );
    
    $hashFin = array(
      date('Ym', strtotime('first day of')) => strtotime('last day of'),
      date('Ym', strtotime('first day of next month')) => strtotime('last day of next month'),
      date('Ym', strtotime('first day of +2 months')) => strtotime('last day of +2 months'),
    );
    
    $ini = $hashIni[$ymonth];
    $fin = $hashFin[$ymonth];

    if (is_numeric($ymonth) && $ymonth != 0) {

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
	   * IMPORTANT: Use field names that appear in admin/config/search/search_api/index/[INDEX_NAME]/fields
	   *
	   * NOTE: Search API stores dates as timestamps
	   *
	   * http://enzolutions.com/articles/2015/06/10/how-to-customize-a-search-api-filters-in-drupal-7/
	   * https://github.com/markaspot/mark-a-spot/blob/master/profiles/markaspot/modules/mark_a_spot/modules/markaspot_radar/markaspot_radar.module
	   */
      $query->addField('field_date:value');
      $query->addField('field_date:value2');
      
      // CASE 1
      $subfilter1 = $query->createFilter('AND');
      $subfilter1->condition('field_date:value', $ini , '>=')
                 ->condition('field_date:value2', $fin, '<=');

      // CASE 2
      $subfilter2 = $query->createFilter('AND');
      $subfilter2->condition('field_date:value', $ini , '<')
                 ->condition('field_date:value2', $fin, '>');

      // CASE 3
      $subfilter3 = $query->createFilter('AND');
      $subfilter3->condition('field_date:value', $ini , '>')
                 ->condition('field_date:value2', $fin, '>')
                 ->condition('field_date:value', $fin , '<=');

      // CASE 4
      $subfilter4 = $query->createFilter('AND');
      $subfilter4->condition('field_date:value', $ini , '<')
                 ->condition('field_date:value2', $fin, '<')
                 ->condition('field_date:value2', $ini , '>=');

      $main_filter = $query->createFilter('OR');
      $main_filter->filter($subfilter1);
      $main_filter->filter($subfilter2);
      $main_filter->filter($subfilter3);
      $main_filter->filter($subfilter4);

      $query->filter($main_filter);

      //dpm($query);
    }
  }
  
  if (!empty($view->exposed_raw_input['years'])) {
    $years = $view->exposed_raw_input['years'];

    $yhash = array( -2 => date('Y') - 2,
      -1 => date('Y') - 1,
      0 => date('Y'),
      1 => date('Y') + 1,
      2 => date('Y') + 2,
    );

    $hashIni = array(
      $yhash[-2] => strtotime('first day of January ' . $yhash[-2] ),
      $yhash[-1] => strtotime('first day of January ' . $yhash[-1] ),
      $yhash[0] => strtotime('first day of January ' . $yhash[0] ),
      $yhash[1] => strtotime('first day of January ' . $yhash[1] ),
      $yhash[2] => strtotime('first day of January ' . $yhash[2] ),
    );
    
    $hashFin = array(      
      $yhash[-2] => strtotime('last day of December ' . $yhash[-2] ),
      $yhash[-1] => strtotime('last day of December ' . $yhash[-1] ),
      $yhash[0] => strtotime('last day of December ' . $yhash[0] ),
      $yhash[1] => strtotime('last day of December ' . $yhash[1] ),
      $yhash[2] => strtotime('last day of December ' . $yhash[2] ),
    );
    
    $ini = $hashIni[$years];
    $fin = $hashFin[$years];

    if (is_numeric($years) && $years != 0) {

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
	   * IMPORTANT: Use field names that appear in admin/config/search/search_api/index/[INDEX_NAME]/fields
	   *
	   * NOTE: Search API stores dates as timestamps
	   */
      $query->addField('field_date:value');
      $query->addField('field_date:value2');
      
      // CASE 1
      $subfilter1 = $query->createFilter('AND');
      $subfilter1->condition('field_date:value', $ini , '>=')
                 ->condition('field_date:value2', $fin, '<=');

      // CASE 2
      $subfilter2 = $query->createFilter('AND');
      $subfilter2->condition('field_date:value', $ini , '<')
                 ->condition('field_date:value2', $fin, '>');

      // CASE 3
      $subfilter3 = $query->createFilter('AND');
      $subfilter3->condition('field_date:value', $ini , '>')
                 ->condition('field_date:value2', $fin, '>')
                 ->condition('field_date:value', $fin , '<=');

      // CASE 4
      $subfilter4 = $query->createFilter('AND');
      $subfilter4->condition('field_date:value', $ini , '<')
                 ->condition('field_date:value2', $fin, '<')
                 ->condition('field_date:value2', $ini , '>=');

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
