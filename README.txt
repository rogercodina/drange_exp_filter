-------------------------
Date range exposed filter
-------------------------

This Drupal 7 module allows you to add exposed date range filters so you can
filter content which matches between initial and final date of a year or a
month. This module works in standart and in Search API views.



            INITIAL          FINAL
               |--------------|


  CASE 1          |--------|

  CASE 2    |----------------------|

  CASE 3                   |--------|

  CASE 4    |--------|


IMPORTANT NOTE: This module is not plug & play. It is meant to be used for
programmers as a base to start with. It assumes that you have a date field
for your content with "field_date" as its machine name. Also, it assumes
this field also stores an end date. Finally, notice you have to expose the
filter, otherwise the query will not be filtered.
