<?php

/**
 * @file
 * Views custom exposed filter interval plugin.
 */

class drange_exp_filter_handler_filter_months_sapi extends views_handler_filter {

  function can_expose() { return TRUE; }

  /**
   * Implements views_object#admin_summary().
   */
  function admin_summary() {
    return t('Filters by whole month selected');
  }

  /**
   * Options form subform for setting exposed filter options.
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['value']['fulld_yearmonth'] = array(
      '#type' => 'date_popup',
      '#date_format' => 'm/Y',
      '#default_value' => "now",
      '#date_year_range' => '-3:3',
      '#size' => 12,
      '#date_label_position' => 'within',
      '#attributes' => array(
        'placeholder' => t('From'),
        'type' => 'date',
      ),
    );
  }

  /**
   * Alters Views query when filter is used.
   * Nomes s'executa si el filtre no es exposat.
   * Per aixo hem implementat el hook_views_query_alter a l'altre fitxer.
   */
  /*function query() {}*/

  /**
   * Define custom option.
   */
  function option_definition() {
     $options = parent::option_definition();
     $options['filter_field_name'] = array('default' => '');
     return $options;
  }

  /**
   * Define a custom option to allow user to set the date fields for the filter.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['filter_field_name'] = array(
     '#type' => 'textfield',
     '#title' => t('Date field with two dates to use for filtering (machine name)'),
     '#default_value' => isset($this->options['filter_field_name']) ? $this->options['filter_field_name'] : '',
     '#description' => t('Ex. field_SOMETHING'),
     '#size' => 60,
     '#maxlength' => 128,
     '#required' => TRUE,
    );
  }

  /**
   * Options save.
   */
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
  }
}
