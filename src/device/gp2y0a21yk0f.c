/*!
 * @file
 * @brief
 */

#include "gp2y0a21yk0f.h"
#include "lookup_table.h"
#include "tiny_utils.h"

static const lookup_table_entry_t entries[] = {
  { 0, 0 }
};

static const lookup_table_t lookup_table = {
  entries,
  element_count(entries)
};

distance_in_cm_t gp2y0a21yk0f_counts_to_distance(tiny_adc_counts_t counts) {
  return (distance_in_cm_t)lookup_table_lookup(&lookup_table, counts);
}
