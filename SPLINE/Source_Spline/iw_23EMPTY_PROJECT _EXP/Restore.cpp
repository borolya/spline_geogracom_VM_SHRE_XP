#include "UI.h"
void restore_to_empty(void)
{
  while ( last_pts!=NULL )
	{
	  curr_pts=last_pts;
	  last_pts=last_pts->prev;
	  delete(curr_pts);
	}
	 n_pts=0;
}

void restore_to_begin(void)
{
  n_pts--;
  while ( last_pts!=NULL )
	{
	  curr_pts=last_pts;
	  last_pts=last_pts->prev;
	  delete(curr_pts);
	  n_pts--;
	}
	trace_pts_first=end_pts;
}

void restore_to_end(void)
{
  n_pts--;
  while ( last_pts!=trace_pts_last )
	{
	  curr_pts=last_pts;
	  last_pts=last_pts->prev;
	  delete(curr_pts);
	  n_pts--;
	}
	trace_pts_last->next=NULL;
}








