/* 
   Copyright (c) 2008 - Chris Buckley. 

   Permission is granted for use and modification of this file for
   research, non-commercial purposes. 
*/

#include "common.h"
#include "sysfunc.h"
#include "trec_eval.h"
#include "functions.h"
#include "trec_format.h"

/* Average over doc pairs of preference ratio for that pair.
   If a doc pair satisfies 3 preferences but fails 2 preferences (preferences
   from 5 different users),  then the score for doc pair is 3/5.
   Same as prefs_simp if there are no doc_pairs in multiple judgment groups.
   For doc pref A>B, includes implied preferences (only one of A or B
   retrieved), and counts as failure if neither A nor B retrieved.
*/

int 
te_calc_prefs_pair (const EPI *epi, const REL_INFO *rel_info,
		    const RESULTS *results, const TREC_MEAS *tm,
		    TREC_EVAL *eval)
{
    RESULTS_PREFS rp;
    long i, j;
    double sum = 0;
    long num_pairs = 0;

    if (UNDEF == form_prefs_counts (epi, rel_info, results, &rp))
	return (UNDEF);

    for (i = 0; i < rp.num_judged_ret; i++) {
	for (j = i+1; j < rp.num_judged_ret; j++) {
	    if (rp.pref_counts.array[i][j] ||
		rp.pref_counts.array[j][i]) {
		num_pairs++;
		sum += (double) rp.pref_counts.array[i][j] /
		    (double) (rp.pref_counts.array[i][j] +
			     rp.pref_counts.array[j][i]);
	    }
	}
    }

    for (i = 0; i < rp.num_judged_ret; i++) {
	for (j = rp.num_judged_ret; j < rp.num_judged; j++) {
	    if (rp.pref_counts.array[i][j] ||
		rp.pref_counts.array[j][i]) {
		num_pairs++;
		sum += (double) rp.pref_counts.array[i][j] /
		    (double) (rp.pref_counts.array[i][j] +
			     rp.pref_counts.array[j][i]);
	    }
	}
    }
    for (i = rp.num_judged_ret; i < rp.num_judged; i++) {
	for (j = i+1; j < rp.num_judged; j++) {
	    if (rp.pref_counts.array[i][j] ||
		rp.pref_counts.array[j][i]) {
		num_pairs++;
	    }
	}
    }

    if (num_pairs) {
        eval->values[tm->eval_index].value =
	    sum / (double) num_pairs;
    }
    return (1);
}