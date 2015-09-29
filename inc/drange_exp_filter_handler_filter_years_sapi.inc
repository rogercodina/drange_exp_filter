<?php

/**
 * @file
 * Views custom exposed filter years plugin.
 */

class drange_exp_filter_handler_filter_years_sapi extends views_handler_filter {

  function can_expose() { return TRUE; }

  /**
   * Implements views_object#admin_summary().
   */
  function admin_summary() {
    return t('Filters by year');
  }

  /**
   * Options form subform for setting exposed filter options.
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $yhash = array( -2 => date('Y') - 2,
      -1 => date('Y') - 1,
      0 => date('Y'),
      1 => date('Y') + 1,
      2 => date('Y') + 2,
    );

    $form['value']['years'] = array(
       '#type' => 'select',
       '#title' => '',
       '#options' => array(
          0 => t('Year'),          
          $yhash[-2] => $yhash[-2],
          $yhash[-1] => $yhash[-1],
          $yhash[0] => $yhash[0],
          $yhash[1] => $yhash[1],
          $yhash[2] => $yhash[2],
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
