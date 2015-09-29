<?php

/**
 * @file
 * Views custom exposed filter months plugin.
 */

class drange_exp_filter_handler_filter_months_sapi extends views_handler_filter {

  function can_expose() { return TRUE; }

  /**
   * Implements views_object#admin_summary().
   */
  function admin_summary() {
    return t('Filters by month and year');
  }

  /**
   * Options form subform for setting exposed filter options.
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['value']['ymonth'] = array(
       '#type' => 'select',
       '#title' => '',
       '#options' => array(
          0 => t('Year'),          
          date('Ym', strtotime('first day of')) => format_date(strtotime('first day of'), 'custom', 'F Y'),
          date('Ym', strtotime('first day of next month')) => format_date(strtotime('first day of next month'), 'custom', 'F Y'),
          date('Ym', strtotime('first day of +2 months')) => format_date(strtotime('first day of +2 months'), 'custom', 'F Y'),
       ),
       '#default_value' => 0,
   );
  }

  /**
   * Alters Views query when filter is used.
   * Nomes s'executa si el filtre no es exposat.
   * Per aixo hem implementat el hook_views_query_alter a l'altre fitxer.
   */
  /*function query() {}*/
}
