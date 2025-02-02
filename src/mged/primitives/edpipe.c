/*                        E D P I P E . C
 * BRL-CAD
 *
 * Copyright (c) 1995-2025 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file mged/primitives/edpipe.c
 *
 * Functions -
 *
 * pipe_split_pnt - split a pipe segment at a given point
 *
 * find_pipe_pnt_nearest_pnt - find which segment of a pipe is nearest
 * the ray from "pt" in the viewing direction (for segment selection
 * in MGED)
 */

#include "common.h"

#include <math.h>
#include <string.h>

#include "vmath.h"
#include "nmg.h"
#include "rt/geom.h"
#include "ged.h"
#include "wdb.h"

#include "../mged.h"
#include "../sedit.h"
#include "../mged_dm.h"
#include "./mged_functab.h"

#define ECMD_PIPE_PICK		15028	/* Pick pipe point */
#define ECMD_PIPE_SPLIT		15029	/* Split a pipe segment into two */
#define ECMD_PIPE_PT_ADD	15030	/* Add a pipe point to end of pipe */
#define ECMD_PIPE_PT_INS	15031	/* Add a pipe point to start of pipe */
#define ECMD_PIPE_PT_DEL	15032	/* Delete a pipe point */
#define ECMD_PIPE_PT_MOVE	15033	/* Move a pipe point */

#define MENU_PIPE_SELECT	15061
#define MENU_PIPE_NEXT_PT	15062
#define MENU_PIPE_PREV_PT	15063
#define MENU_PIPE_SPLIT		15064
#define MENU_PIPE_PT_OD		15065
#define MENU_PIPE_PT_ID		15066
#define MENU_PIPE_SCALE_OD	15067
#define MENU_PIPE_SCALE_ID	15068
#define MENU_PIPE_ADD_PT	15069
#define MENU_PIPE_INS_PT	15070
#define MENU_PIPE_DEL_PT	15071
#define MENU_PIPE_MOV_PT	15072
#define MENU_PIPE_PT_RADIUS	15073
#define MENU_PIPE_SCALE_RADIUS	15074

struct wdb_pipe_pnt *es_pipe_pnt=(struct wdb_pipe_pnt *)NULL; /* Currently selected PIPE segment */

static void
pipe_ed(struct mged_state *s, int arg, int UNUSED(a), int UNUSED(b), void *UNUSED(data))
{
    struct wdb_pipe_pnt *next;
    struct wdb_pipe_pnt *prev;

    if (s->dbip == DBI_NULL)
	return;

    switch (arg) {
	case MENU_PIPE_SELECT:
	    s->s_edit->edit_menu = arg;
	    s->s_edit->edit_flag = ECMD_PIPE_PICK;
	    s->s_edit->solid_edit_rotate = 0;
	    s->s_edit->solid_edit_translate = 0;
	    s->s_edit->solid_edit_scale = 0;
	    s->s_edit->solid_edit_pick = 1;
	    break;
	case MENU_PIPE_NEXT_PT:
	    if (!es_pipe_pnt) {
		bu_vls_printf(s->s_edit->log_str, "No Pipe Segment selected\n");
		return;
	    }
	    next = BU_LIST_NEXT(wdb_pipe_pnt, &es_pipe_pnt->l);
	    if (next->l.magic == BU_LIST_HEAD_MAGIC) {
		bu_vls_printf(s->s_edit->log_str, "Current segment is the last\n");
		return;
	    }
	    es_pipe_pnt = next;
	    rt_pipe_pnt_print(es_pipe_pnt, s->dbip->dbi_base2local);
	    s->s_edit->edit_menu = arg;
	    mged_set_edflag(s, IDLE);
	    sedit(s);
	    break;
	case MENU_PIPE_PREV_PT:
	    if (!es_pipe_pnt) {
		bu_vls_printf(s->s_edit->log_str, "No Pipe Segment selected\n");
		return;
	    }
	    prev = BU_LIST_PREV(wdb_pipe_pnt, &es_pipe_pnt->l);
	    if (prev->l.magic == BU_LIST_HEAD_MAGIC) {
		bu_vls_printf(s->s_edit->log_str, "Current segment is the first\n");
		return;
	    }
	    es_pipe_pnt = prev;
	    rt_pipe_pnt_print(es_pipe_pnt, s->dbip->dbi_base2local);
	    s->s_edit->edit_menu = arg;
	    mged_set_edflag(s, IDLE);
	    sedit(s);
	    break;
	case MENU_PIPE_SPLIT:
	    /* not used */
#if 0
	    s->s_edit->edit_flag = ECMD_PIPE_SPLIT;
	    s->s_edit->solid_edit_rotate = 0;
	    s->s_edit->solid_edit_translate = 1;
	    s->s_edit->solid_edit_scale = 0;
	    s->s_edit->solid_edit_pick = 0;
#endif
	    break;
	case MENU_PIPE_MOV_PT:
	    if (!es_pipe_pnt) {
		bu_vls_printf(s->s_edit->log_str, "No Pipe Segment selected\n");
		mged_set_edflag(s, IDLE);
		return;
	    }
	    s->s_edit->edit_menu = arg;
	    s->s_edit->edit_flag = ECMD_PIPE_PT_MOVE;
	    s->s_edit->solid_edit_rotate = 0;
	    s->s_edit->solid_edit_translate = 1;
	    s->s_edit->solid_edit_scale = 0;
	    s->s_edit->solid_edit_pick = 0;
	    break;
	case MENU_PIPE_PT_OD:
	case MENU_PIPE_PT_ID:
	case MENU_PIPE_PT_RADIUS:
	    if (!es_pipe_pnt) {
		bu_vls_printf(s->s_edit->log_str, "No Pipe Segment selected\n");
		mged_set_edflag(s, IDLE);
		return;
	    }
	    s->s_edit->edit_menu = arg;
	    mged_set_edflag(s, PSCALE);
	    break;
	case MENU_PIPE_SCALE_OD:
	case MENU_PIPE_SCALE_ID:
	case MENU_PIPE_SCALE_RADIUS:
	    s->s_edit->edit_menu = arg;
	    mged_set_edflag(s, PSCALE);
	    break;
	case MENU_PIPE_ADD_PT:
	    s->s_edit->edit_menu = arg;
	    s->s_edit->edit_flag = ECMD_PIPE_PT_ADD;
	    s->s_edit->solid_edit_rotate = 0;
	    s->s_edit->solid_edit_translate = 1;
	    s->s_edit->solid_edit_scale = 0;
	    s->s_edit->solid_edit_pick = 0;
	    break;
	case MENU_PIPE_INS_PT:
	    s->s_edit->edit_menu = arg;
	    s->s_edit->edit_flag = ECMD_PIPE_PT_INS;
	    s->s_edit->solid_edit_rotate = 0;
	    s->s_edit->solid_edit_translate = 1;
	    s->s_edit->solid_edit_scale = 0;
	    s->s_edit->solid_edit_pick = 0;
	    break;
	case MENU_PIPE_DEL_PT:
	    s->s_edit->edit_menu = arg;
	    mged_set_edflag(s, ECMD_PIPE_PT_DEL);
	    sedit(s);
	    break;
    }
    set_e_axes_pos(s, 1);
}


struct menu_item pipe_menu[] = {
    { "PIPE MENU", NULL, 0 },
    { "Select Point", pipe_ed, MENU_PIPE_SELECT },
    { "Next Point", pipe_ed, MENU_PIPE_NEXT_PT },
    { "Previous Point", pipe_ed, MENU_PIPE_PREV_PT },
    { "Move Point", pipe_ed, MENU_PIPE_MOV_PT },
    { "Delete Point", pipe_ed, MENU_PIPE_DEL_PT },
    { "Append Point", pipe_ed, MENU_PIPE_ADD_PT },
    { "Prepend Point", pipe_ed, MENU_PIPE_INS_PT },
    { "Set Point OD", pipe_ed, MENU_PIPE_PT_OD },
    { "Set Point ID", pipe_ed, MENU_PIPE_PT_ID },
    { "Set Point Bend", pipe_ed, MENU_PIPE_PT_RADIUS },
    { "Set Pipe OD", pipe_ed, MENU_PIPE_SCALE_OD },
    { "Set Pipe ID", pipe_ed, MENU_PIPE_SCALE_ID },
    { "Set Pipe Bend", pipe_ed, MENU_PIPE_SCALE_RADIUS },
    { "", NULL, 0 }
};

struct menu_item *
mged_pipe_menu_item(const struct bn_tol *UNUSED(tol))
{
    return pipe_menu;
}


void
pipe_split_pnt(struct bu_list *UNUSED(pipe_hd), struct wdb_pipe_pnt *UNUSED(ps), point_t UNUSED(new_pt))
{
    bu_log("WARNING: pipe splitting unimplemented\n");
}


void
pipe_scale_od(struct mged_state *s, struct rt_db_internal *db_int, fastf_t scale)
{
    struct wdb_pipe_pnt *ps;
    struct rt_pipe_internal *pipeip=(struct rt_pipe_internal *)db_int->idb_ptr;

    RT_PIPE_CK_MAGIC(pipeip);

    /* check that this can be done */
    for (BU_LIST_FOR(ps, wdb_pipe_pnt, &pipeip->pipe_segs_head)) {
	fastf_t tmp_od;

	if (scale < 0.0)
	    tmp_od = (-scale);
	else
	    tmp_od = ps->pp_od*scale;
	if (ps->pp_id > tmp_od) {
	    bu_vls_printf(s->s_edit->log_str, "Cannot make OD less than ID\n");
	    return;
	}
	if (tmp_od > 2.0*ps->pp_bendradius) {
	    bu_vls_printf(s->s_edit->log_str, "Cannot make outer radius greater than bend radius\n");
	    return;
	}
    }

    for (BU_LIST_FOR(ps, wdb_pipe_pnt, &pipeip->pipe_segs_head))
	ps->pp_od *= scale;
}


void
pipe_scale_id(struct mged_state *s, struct rt_db_internal *db_int, fastf_t scale)
{
    struct wdb_pipe_pnt *ps;
    struct rt_pipe_internal *pipeip=(struct rt_pipe_internal *)db_int->idb_ptr;
    fastf_t tmp_id;

    RT_PIPE_CK_MAGIC(pipeip);

    /* check that this can be done */
    for (BU_LIST_FOR(ps, wdb_pipe_pnt, &pipeip->pipe_segs_head)) {
	if (scale > 0.0)
	    tmp_id = ps->pp_id*scale;
	else
	    tmp_id = (-scale);
	if (ps->pp_od < tmp_id) {
	    bu_vls_printf(s->s_edit->log_str, "Cannot make ID greater than OD\n");
	    return;
	}
	if (tmp_id > 2.0*ps->pp_bendradius) {
	    bu_vls_printf(s->s_edit->log_str, "Cannot make inner radius greater than bend radius\n");
	    return;
	}
    }

    for (BU_LIST_FOR(ps, wdb_pipe_pnt, &pipeip->pipe_segs_head)) {
	if (scale > 0.0)
	    ps->pp_id *= scale;
	else
	    ps->pp_id = (-scale);
    }
}


void
pipe_seg_scale_od(struct mged_state *s, struct wdb_pipe_pnt *ps, fastf_t scale)
{
    fastf_t tmp_od;

    BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "pipe segment");

    /* need to check that the new OD is not less than ID
     * of any affected segment.
     */
    if (scale < 0.0)
	tmp_od = (-scale);
    else
	tmp_od = scale*ps->pp_od;
    if (ps->pp_id > tmp_od) {
	bu_vls_printf(s->s_edit->log_str, "Cannot make OD smaller than ID\n");
	return;
    }
    if (tmp_od > 2.0*ps->pp_bendradius) {
	bu_vls_printf(s->s_edit->log_str, "Cannot make outer radius greater than bend radius\n");
	return;
    }

    if (scale > 0.0)
	ps->pp_od *= scale;
    else
	ps->pp_od = (-scale);
}


void
pipe_seg_scale_id(struct mged_state *s, struct wdb_pipe_pnt *ps, fastf_t scale)
{
    fastf_t tmp_id;

    BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "pipe segment");

    /* need to check that the new ID is not greater than OD */
    if (scale > 0.0)
	tmp_id = scale*ps->pp_id;
    else
	tmp_id = (-scale);
    if (ps->pp_od < tmp_id) {
	bu_vls_printf(s->s_edit->log_str, "Cannot make ID greater than OD\n");
	return;
    }
    if (tmp_id > 2.0*ps->pp_bendradius) {
	bu_vls_printf(s->s_edit->log_str, "Cannot make inner radius greater than bend radius\n");
	return;
    }

    if (scale > 0.0)
	ps->pp_id *= scale;
    else
	ps->pp_id = (-scale);
}


void
pipe_seg_scale_radius(struct mged_state *s, struct wdb_pipe_pnt *ps, fastf_t scale)
{
    fastf_t old_radius;
    struct wdb_pipe_pnt *head;

    BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "pipe point");

    head = ps;
    while (head->l.magic != BU_LIST_HEAD_MAGIC)
	head = BU_LIST_NEXT(wdb_pipe_pnt, &head->l);

    /* make sure we can make this change */
    old_radius = ps->pp_bendradius;
    if (scale > 0.0)
	ps->pp_bendradius *= scale;
    else
	ps->pp_bendradius = (-scale);

    if (ps->pp_bendradius < ps->pp_od * 0.5) {
	bu_vls_printf(s->s_edit->log_str, "Cannot make bend radius less than pipe outer radius\n");
	ps->pp_bendradius = old_radius;
	return;
    }

    if (rt_pipe_ck(&head->l)) {
	/* won't work, go back to original radius */
	ps->pp_bendradius = old_radius;
	return;
    }

}


void
pipe_scale_radius(struct mged_state *s, struct rt_db_internal *db_int, fastf_t scale)
{
    struct bu_list head;
    struct wdb_pipe_pnt *old_ps, *new_ps;
    struct rt_pipe_internal *pipeip=(struct rt_pipe_internal *)db_int->idb_ptr;

    RT_CK_DB_INTERNAL(db_int);
    RT_PIPE_CK_MAGIC(pipeip);

    /* make a quick check for minimum bend radius */
    for (BU_LIST_FOR(old_ps, wdb_pipe_pnt, &pipeip->pipe_segs_head)) {
	if (scale < 0.0) {
	    if ((-scale) < old_ps->pp_od * 0.5) {
		bu_vls_printf(s->s_edit->log_str, "Cannot make bend radius less than pipe outer radius\n");
		return;
	    }
	} else {
	    if (old_ps->pp_bendradius * scale < old_ps->pp_od * 0.5) {
		bu_vls_printf(s->s_edit->log_str, "Cannot make bend radius less than pipe outer radius\n");
		return;
	    }
	}
    }

    /* make temporary copy of this pipe solid */
    BU_LIST_INIT(&head);
    for (BU_LIST_FOR(old_ps, wdb_pipe_pnt, &pipeip->pipe_segs_head)) {
	BU_ALLOC(new_ps, struct wdb_pipe_pnt);
	*new_ps = (*old_ps);
	BU_LIST_APPEND(&head, &new_ps->l);
    }

    /* make the desired editing changes to the copy */
    for (BU_LIST_FOR(new_ps, wdb_pipe_pnt, &head)) {
	if (scale < 0.0)
	    new_ps->pp_bendradius = (-scale);
	else
	    new_ps->pp_bendradius *= scale;
    }

    /* check if the copy is O.K. */
    if (rt_pipe_ck(&head)) {
	/* won't work, go back to original */
	while (BU_LIST_NON_EMPTY(&head)) {
	    new_ps = BU_LIST_FIRST(wdb_pipe_pnt, &head);
	    BU_LIST_DEQUEUE(&new_ps->l);
	    bu_free((void *)new_ps, "pipe_scale_radius: new_ps");
	}
	return;
    }

    /* free temporary pipe solid */
    while (BU_LIST_NON_EMPTY(&head)) {
	new_ps = BU_LIST_FIRST(wdb_pipe_pnt, &head);
	BU_LIST_DEQUEUE(&new_ps->l);
	bu_free((void *)new_ps, "pipe_scale_radius: new_ps");
    }

    /* make changes to the original */
    for (BU_LIST_FOR(old_ps, wdb_pipe_pnt, &pipeip->pipe_segs_head)) {
	if (scale < 0.0)
	    old_ps->pp_bendradius = (-scale);
	else
	    old_ps->pp_bendradius *= scale;
    }

}


struct wdb_pipe_pnt *
find_pipe_pnt_nearest_pnt(struct mged_state *s, const struct bu_list *pipe_hd, const point_t pt)
{
    struct wdb_pipe_pnt *ps;
    struct wdb_pipe_pnt *nearest=(struct wdb_pipe_pnt *)NULL;
    struct bn_tol tmp_tol;
    fastf_t min_dist = MAX_FASTF;
    vect_t dir, work;

    tmp_tol.magic = BN_TOL_MAGIC;
    tmp_tol.dist = 0.0;
    tmp_tol.dist_sq = tmp_tol.dist * tmp_tol.dist;
    tmp_tol.perp = 0.0;
    tmp_tol.para = 1.0 - tmp_tol.perp;

    /* get a direction vector in model space corresponding to z-direction in view */
    VSET(work, 0.0, 0.0, 1.0);
    MAT4X3VEC(dir, view_state->vs_gvp->gv_view2model, work);

    for (BU_LIST_FOR(ps, wdb_pipe_pnt, pipe_hd)) {
	fastf_t dist;

	dist = bg_dist_line3_pnt3(pt, dir, ps->pp_coord);
	if (dist < min_dist) {
	    min_dist = dist;
	    nearest = ps;
	}
    }
    return nearest;
}


struct wdb_pipe_pnt *
pipe_add_pnt(struct rt_pipe_internal *pipeip, struct wdb_pipe_pnt *pp, const point_t new_pt)
{
    struct wdb_pipe_pnt *last;
    struct wdb_pipe_pnt *newpp;

    RT_PIPE_CK_MAGIC(pipeip);
    if (pp)
	BU_CKMAG(pp, WDB_PIPESEG_MAGIC, "pipe point");

    if (pp)
	last = pp;
    else {
	/* add new point to end of pipe solid */
	last = BU_LIST_LAST(wdb_pipe_pnt, &pipeip->pipe_segs_head);
	if (last->l.magic == BU_LIST_HEAD_MAGIC) {
	    BU_ALLOC(newpp, struct wdb_pipe_pnt);
	    newpp->l.magic = WDB_PIPESEG_MAGIC;
	    newpp->pp_od = 30.0;
	    newpp->pp_id = 0.0;
	    newpp->pp_bendradius = 40.0;
	    VMOVE(newpp->pp_coord, new_pt);
	    BU_LIST_INSERT(&pipeip->pipe_segs_head, &newpp->l);
	    return newpp;
	}
    }

    /* build new point */
    BU_ALLOC(newpp, struct wdb_pipe_pnt);
    newpp->l.magic = WDB_PIPESEG_MAGIC;
    newpp->pp_od = last->pp_od;
    newpp->pp_id = last->pp_id;
    newpp->pp_bendradius = last->pp_bendradius;
    VMOVE(newpp->pp_coord, new_pt);

    if (!pp)	/* add to end of pipe solid */
	BU_LIST_INSERT(&pipeip->pipe_segs_head, &newpp->l)
	    else		/* append after current point */
		BU_LIST_APPEND(&pp->l, &newpp->l)

		    if (rt_pipe_ck(&pipeip->pipe_segs_head)) {
			/* won't work here, so refuse to do it */
			BU_LIST_DEQUEUE(&newpp->l);
			bu_free((void *)newpp, "pipe_add_pnt: newpp ");
			return pp;
		    } else
			return newpp;
}


void
pipe_ins_pnt(struct rt_pipe_internal *pipeip, struct wdb_pipe_pnt *pp, const point_t new_pt)
{
    struct wdb_pipe_pnt *first;
    struct wdb_pipe_pnt *newpp;

    RT_PIPE_CK_MAGIC(pipeip);
    if (pp)
	BU_CKMAG(pp, WDB_PIPESEG_MAGIC, "pipe point");

    if (pp)
	first = pp;
    else {
	/* insert new point at start of pipe solid */
	first = BU_LIST_FIRST(wdb_pipe_pnt, &pipeip->pipe_segs_head);
	if (first->l.magic == BU_LIST_HEAD_MAGIC) {
	    BU_ALLOC(newpp, struct wdb_pipe_pnt);
	    newpp->l.magic = WDB_PIPESEG_MAGIC;
	    newpp->pp_od = 30.0;
	    newpp->pp_id = 0.0;
	    newpp->pp_bendradius = 40.0;
	    VMOVE(newpp->pp_coord, new_pt);
	    BU_LIST_APPEND(&pipeip->pipe_segs_head, &newpp->l);
	    return;
	}
    }

    /* build new point */
    BU_ALLOC(newpp, struct wdb_pipe_pnt);
    newpp->l.magic = WDB_PIPESEG_MAGIC;
    newpp->pp_od = first->pp_od;
    newpp->pp_id = first->pp_id;
    newpp->pp_bendradius = first->pp_bendradius;
    VMOVE(newpp->pp_coord, new_pt);

    if (!pp)	/* add to start of pipe */
	BU_LIST_APPEND(&pipeip->pipe_segs_head, &newpp->l)
	    else		/* insert before current point */
		BU_LIST_INSERT(&pp->l, &newpp->l)

		    if (rt_pipe_ck(&pipeip->pipe_segs_head)) {
			/* won't work here, so refuse to do it */
			BU_LIST_DEQUEUE(&newpp->l);
			bu_free((void *)newpp, "pipe_ins_pnt: newpp ");
		    }
}


struct wdb_pipe_pnt *
pipe_del_pnt(struct mged_state *s, struct wdb_pipe_pnt *ps)
{
    struct wdb_pipe_pnt *next;
    struct wdb_pipe_pnt *prev;
    struct wdb_pipe_pnt *head;

    BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "pipe segment");

    head = ps;
    while (head->l.magic != BU_LIST_HEAD_MAGIC)
	head = BU_LIST_NEXT(wdb_pipe_pnt, &head->l);

    next = BU_LIST_NEXT(wdb_pipe_pnt, &ps->l);
    if (next->l.magic == BU_LIST_HEAD_MAGIC)
	next = (struct wdb_pipe_pnt *)NULL;

    prev = BU_LIST_PREV(wdb_pipe_pnt, &ps->l);
    if (prev->l.magic == BU_LIST_HEAD_MAGIC)
	prev = (struct wdb_pipe_pnt *)NULL;

    if (!prev && !next) {
	bu_vls_printf(s->s_edit->log_str, "Cannot delete last point in pipe\n");
	return ps;
    }

    BU_LIST_DEQUEUE(&ps->l);

    if (rt_pipe_ck(&head->l)) {
	bu_vls_printf(s->s_edit->log_str, "Cannot delete this point, it will result in an illegal pipe\n");
	if (next)
	    BU_LIST_INSERT(&next->l, &ps->l)
		else if (prev)
		    BU_LIST_APPEND(&prev->l, &ps->l)
			else
			    BU_LIST_INSERT(&head->l, &ps->l)

				return ps;
    } else
	bu_free((void *)ps, "pipe_del_pnt: ps");

    if (prev)
	return prev;
    else
	return next;

}


void
pipe_move_pnt(struct mged_state *s, struct rt_pipe_internal *pipeip, struct wdb_pipe_pnt *ps, const point_t new_pt)
{
    point_t old_pt;

    RT_PIPE_CK_MAGIC(pipeip);
    BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "pipe segment");

    VMOVE(old_pt, ps->pp_coord);

    VMOVE(ps->pp_coord, new_pt);
    if (rt_pipe_ck(&pipeip->pipe_segs_head)) {
	bu_vls_printf(s->s_edit->log_str, "Cannot move point there\n");
	VMOVE(ps->pp_coord, old_pt);
    }
}

const char *
mged_pipe_keypoint(
	point_t *pt,
	const char *UNUSED(keystr),
	const mat_t mat,
	const struct rt_db_internal *ip,
	const struct bn_tol *UNUSED(tol))
{
    static const char *strp = "V";
    point_t mpt = VINIT_ZERO;
    RT_CK_DB_INTERNAL(ip);
    struct rt_pipe_internal *pipeip =(struct rt_pipe_internal *)ip->idb_ptr;
    struct wdb_pipe_pnt *pipe_seg;
    RT_PIPE_CK_MAGIC(pipeip);
    if (es_pipe_pnt == (struct wdb_pipe_pnt *)NULL) {
	pipe_seg = BU_LIST_FIRST(wdb_pipe_pnt, &pipeip->pipe_segs_head);
	VMOVE(mpt, pipe_seg->pp_coord);
    } else {
	VMOVE(mpt, es_pipe_pnt->pp_coord);
    }
    MAT4X3PNT(*pt, mat, mpt);
    return strp;
}

void
mged_pipe_labels(
	int *UNUSED(num_lines),
	point_t *UNUSED(lines),
	struct rt_point_labels *pl,
	int UNUSED(max_pl),
	const mat_t xform,
	struct rt_db_internal *ip,
	struct bn_tol *UNUSED(tol))
{
    point_t pos_view;
    int npl = 0;


#define POINT_LABEL_STR(_pt, _str) { \
    VMOVE(pl[npl].pt, _pt); \
    bu_strlcpy(pl[npl++].str, _str, sizeof(pl[0].str)); }

    RT_CK_DB_INTERNAL(ip);
    //struct rt_pipe_internal *pipeip = (struct rt_pipe_internal *)ip->idb_ptr;
    //RT_PIPE_CK_MAGIC(pipeip);

    // Conditional labeling
    if (es_pipe_pnt) {
	BU_CKMAG(es_pipe_pnt, WDB_PIPESEG_MAGIC, "wdb_pipe_pnt");

	MAT4X3PNT(pos_view, xform, es_pipe_pnt->pp_coord);
	POINT_LABEL_STR(pos_view, "pt");
    }

    pl[npl].str[0] = '\0';	/* Mark ending */
}

/* scale OD of one pipe segment */
int
menu_pipe_pt_od(struct mged_state *s)
{

    if (s->s_edit->e_para[0] < 0.0) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: SCALE FACTOR < 0\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "pscale: no pipe segment selected for scaling\n");
	return TCL_ERROR;
    }

    if (s->s_edit->e_inpara) {
	/* take s->s_edit->e_mat[15] (path scaling) into account */
	if (es_pipe_pnt->pp_od > 0.0)
	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/es_pipe_pnt->pp_od;
	else
	    s->s_edit->es_scale = (-s->s_edit->e_para[0] * s->s_edit->e_mat[15]);
    }
    pipe_seg_scale_od(s, es_pipe_pnt, s->s_edit->es_scale);

    return 0;
}

/* scale ID of one pipe segment */
int
menu_pipe_pt_id(struct mged_state *s)
{
    if (s->s_edit->e_para[0] < 0.0) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: SCALE FACTOR < 0\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "pscale: no pipe segment selected for scaling\n");
	return TCL_ERROR;
    }

    if (s->s_edit->e_inpara) {
	/* take s->s_edit->e_mat[15] (path scaling) into account */
	if (es_pipe_pnt->pp_id > 0.0)
	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/es_pipe_pnt->pp_id;
	else
	    s->s_edit->es_scale = (-s->s_edit->e_para[0] * s->s_edit->e_mat[15]);
    }

    pipe_seg_scale_id(s, es_pipe_pnt, s->s_edit->es_scale);

    return 0;
}

/* scale bend radius at selected point */
int
menu_pipe_pt_radius(struct mged_state *s)
{
    if (s->s_edit->e_para[0] <= 0.0) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: SCALE FACTOR <= 0\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "pscale: no pipe segment selected for scaling\n");
	return TCL_ERROR;
    }

    if (s->s_edit->e_inpara) {
	/* take s->s_edit->e_mat[15] (path scaling) into account */
	if (es_pipe_pnt->pp_id > 0.0)
	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/es_pipe_pnt->pp_bendradius;
	else
	    s->s_edit->es_scale = (-s->s_edit->e_para[0] * s->s_edit->e_mat[15]);
    }

    pipe_seg_scale_radius(s, es_pipe_pnt, s->s_edit->es_scale);

    return 0;
}

/* scale entire pipe OD */
int
menu_pipe_scale_od(struct mged_state *s)
{
    if (s->s_edit->e_para[0] <= 0.0) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: SCALE FACTOR <= 0\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    if (s->s_edit->e_inpara) {
	struct rt_pipe_internal *pipeip =
	    (struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
	struct wdb_pipe_pnt *ps;

	RT_PIPE_CK_MAGIC(pipeip);

	ps = BU_LIST_FIRST(wdb_pipe_pnt, &pipeip->pipe_segs_head);
	BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "wdb_pipe_pnt");

	if (ps->pp_od > 0.0) {
	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/ps->pp_od;
	} else {
	    while (ps->l.magic != BU_LIST_HEAD_MAGIC && ps->pp_od <= 0.0)
		ps = BU_LIST_NEXT(wdb_pipe_pnt, &ps->l);

	    if (ps->l.magic == BU_LIST_HEAD_MAGIC) {
		bu_vls_printf(s->s_edit->log_str, "Entire pipe solid has zero OD!\n");
		return TCL_ERROR;
	    }

	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/ps->pp_od;
	}
    }

    pipe_scale_od(s, &s->s_edit->es_int, s->s_edit->es_scale);

    return 0;
}

/* scale entire pipe ID */
int
menu_pipe_scale_id(struct mged_state *s)
{
    if (s->s_edit->e_para[0] < 0.0) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: SCALE FACTOR < 0\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    if (s->s_edit->e_inpara) {
	struct rt_pipe_internal *pipeip =
	    (struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
	struct wdb_pipe_pnt *ps;

	RT_PIPE_CK_MAGIC(pipeip);

	ps = BU_LIST_FIRST(wdb_pipe_pnt, &pipeip->pipe_segs_head);
	BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "wdb_pipe_pnt");

	if (ps->pp_id > 0.0) {
	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/ps->pp_id;
	} else {
	    while (ps->l.magic != BU_LIST_HEAD_MAGIC && ps->pp_id <= 0.0)
		ps = BU_LIST_NEXT(wdb_pipe_pnt, &ps->l);

	    /* Check if entire pipe has zero ID */
	    if (ps->l.magic == BU_LIST_HEAD_MAGIC)
		s->s_edit->es_scale = (-s->s_edit->e_para[0] * s->s_edit->e_mat[15]);
	    else
		s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/ps->pp_id;
	}
    }
    pipe_scale_id(s, &s->s_edit->es_int, s->s_edit->es_scale);

    return 0;
}

/* scale entire pipe bend radius */
int
menu_pipe_scale_radius(struct mged_state *s)
{
    if (s->s_edit->e_para[0] <= 0.0) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: SCALE FACTOR <= 0\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    if (s->s_edit->e_inpara) {
	struct rt_pipe_internal *pipeip =
	    (struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
	struct wdb_pipe_pnt *ps;

	RT_PIPE_CK_MAGIC(pipeip);

	ps = BU_LIST_FIRST(wdb_pipe_pnt, &pipeip->pipe_segs_head);
	BU_CKMAG(ps, WDB_PIPESEG_MAGIC, "wdb_pipe_pnt");

	if (ps->pp_bendradius > 0.0) {
	    s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/ps->pp_bendradius;
	} else {
	    while (ps->l.magic != BU_LIST_HEAD_MAGIC && ps->pp_bendradius <= 0.0)
		ps = BU_LIST_NEXT(wdb_pipe_pnt, &ps->l);

	    /* Check if entire pipe has zero ID */
	    if (ps->l.magic == BU_LIST_HEAD_MAGIC)
		s->s_edit->es_scale = (-s->s_edit->e_para[0] * s->s_edit->e_mat[15]);
	    else
		s->s_edit->es_scale = s->s_edit->e_para[0] * s->s_edit->e_mat[15]/ps->pp_bendradius;
	}
    }

    pipe_scale_radius(s, &s->s_edit->es_int, s->s_edit->es_scale);

    return 0;
}

void ecmd_pipe_pick(struct mged_state *s)
{
    struct rt_pipe_internal *pipeip =
	(struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
    point_t new_pt;

    RT_PIPE_CK_MAGIC(pipeip);

    /* must convert to base units */
    s->s_edit->e_para[0] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[1] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[2] *= s->dbip->dbi_local2base;

    if (s->s_edit->e_mvalid) {
	VMOVE(new_pt, s->s_edit->e_mparam);
    } else if (s->s_edit->e_inpara == 3) {
	if (s->s_edit->mv_context) {
	    /* apply s->s_edit->e_invmat to convert to real model space */
	    MAT4X3PNT(new_pt, s->s_edit->e_invmat, s->s_edit->e_para);
	} else {
	    VMOVE(new_pt, s->s_edit->e_para);
	}
    } else if (s->s_edit->e_inpara && s->s_edit->e_inpara != 3) {
	bu_vls_printf(s->s_edit->log_str, "x y z coordinates required for segment selection\n");
	mged_print_result(s, TCL_ERROR);
	return;
    } else if (!s->s_edit->e_mvalid && !s->s_edit->e_inpara)
	return;

    es_pipe_pnt = find_pipe_pnt_nearest_pnt(s, &pipeip->pipe_segs_head, new_pt);
    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "No PIPE segment selected\n");
	mged_print_result(s, TCL_ERROR);
    } else
	rt_pipe_pnt_print(es_pipe_pnt, s->dbip->dbi_base2local);
}

void ecmd_pipe_split(struct mged_state *s)
{
    struct rt_pipe_internal *pipeip =
	(struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
    point_t new_pt;

    RT_PIPE_CK_MAGIC(pipeip);

    /* must convert to base units */
    s->s_edit->e_para[0] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[1] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[2] *= s->dbip->dbi_local2base;

    if (s->s_edit->e_mvalid) {
	VMOVE(new_pt, s->s_edit->e_mparam);
    } else if (s->s_edit->e_inpara == 3) {
	if (s->s_edit->mv_context) {
	    /* apply s->s_edit->e_invmat to convert to real model space */
	    MAT4X3PNT(new_pt, s->s_edit->e_invmat, s->s_edit->e_para);
	} else {
	    VMOVE(new_pt, s->s_edit->e_para);
	}
    } else if (s->s_edit->e_inpara && s->s_edit->e_inpara != 3) {
	bu_vls_printf(s->s_edit->log_str, "x y z coordinates required for segment split\n");
	mged_print_result(s, TCL_ERROR);
	return;
    } else if (!s->s_edit->e_mvalid && !s->s_edit->e_inpara)
	return;

    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "No pipe segment selected\n");
	mged_print_result(s, TCL_ERROR);
	return;
    }

    pipe_split_pnt(&pipeip->pipe_segs_head, es_pipe_pnt, new_pt);
}

void ecmd_pipe_pt_move(struct mged_state *s)
{
    struct rt_pipe_internal *pipeip =
	(struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
    point_t new_pt;

    RT_PIPE_CK_MAGIC(pipeip);

    /* must convert to base units */
    s->s_edit->e_para[0] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[1] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[2] *= s->dbip->dbi_local2base;

    if (s->s_edit->e_mvalid) {
	VMOVE(new_pt, s->s_edit->e_mparam);
    } else if (s->s_edit->e_inpara == 3) {
	if (s->s_edit->mv_context) {
	    /* apply s->s_edit->e_invmat to convert to real model space */
	    MAT4X3PNT(new_pt, s->s_edit->e_invmat, s->s_edit->e_para);
	} else {
	    VMOVE(new_pt, s->s_edit->e_para);
	}
    } else if (s->s_edit->e_inpara && s->s_edit->e_inpara != 3) {
	bu_vls_printf(s->s_edit->log_str, "x y z coordinates required for segment movement\n");
	mged_print_result(s, TCL_ERROR);
	return;
    } else if (!s->s_edit->e_mvalid && !s->s_edit->e_inpara)
	return;

    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "No pipe segment selected\n");
	mged_print_result(s, TCL_ERROR);
	return;
    }

    pipe_move_pnt(s, pipeip, es_pipe_pnt, new_pt);
}

void ecmd_pipe_pt_add(struct mged_state *s)
{
    struct rt_pipe_internal *pipeip =
	(struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
    point_t new_pt;

    RT_PIPE_CK_MAGIC(pipeip);

    /* must convert to base units */
    s->s_edit->e_para[0] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[1] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[2] *= s->dbip->dbi_local2base;

    if (s->s_edit->e_mvalid) {
	VMOVE(new_pt, s->s_edit->e_mparam);
    } else if (s->s_edit->e_inpara == 3) {
	if (s->s_edit->mv_context) {
	    /* apply s->s_edit->e_invmat to convert to real model space */
	    MAT4X3PNT(new_pt, s->s_edit->e_invmat, s->s_edit->e_para);
	} else {
	    VMOVE(new_pt, s->s_edit->e_para);
	}
    } else if (s->s_edit->e_inpara && s->s_edit->e_inpara != 3) {
	bu_vls_printf(s->s_edit->log_str, "x y z coordinates required for 'append segment'\n");
	mged_print_result(s, TCL_ERROR);
	return;
    } else if (!s->s_edit->e_mvalid && !s->s_edit->e_inpara)
	return;

    es_pipe_pnt = pipe_add_pnt(pipeip, es_pipe_pnt, new_pt);
}

void ecmd_pipe_pt_ins(struct mged_state *s)
{
    struct rt_pipe_internal *pipeip =
	(struct rt_pipe_internal *)s->s_edit->es_int.idb_ptr;
    point_t new_pt;

    RT_PIPE_CK_MAGIC(pipeip);

    /* must convert to base units */
    s->s_edit->e_para[0] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[1] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[2] *= s->dbip->dbi_local2base;

    if (s->s_edit->e_mvalid) {
	VMOVE(new_pt, s->s_edit->e_mparam);
    } else if (s->s_edit->e_inpara == 3) {
	if (s->s_edit->mv_context) {
	    /* apply s->s_edit->e_invmat to convert to real model space */
	    MAT4X3PNT(new_pt, s->s_edit->e_invmat, s->s_edit->e_para);
	} else {
	    VMOVE(new_pt, s->s_edit->e_para);
	}
    } else if (s->s_edit->e_inpara && s->s_edit->e_inpara != 3) {
	bu_vls_printf(s->s_edit->log_str, "x y z coordinates required for 'prepend segment'\n");
	mged_print_result(s, TCL_ERROR);
	return;
    } else if (!s->s_edit->e_mvalid && !s->s_edit->e_inpara)
	return;

    pipe_ins_pnt(pipeip, es_pipe_pnt, new_pt);
}

void ecmd_pipe_pt_del(struct mged_state *s)
{
    if (!es_pipe_pnt) {
	bu_vls_printf(s->s_edit->log_str, "No pipe segment selected\n");
	mged_print_result(s, TCL_ERROR);
	return;
    }
    es_pipe_pnt = pipe_del_pnt(s, es_pipe_pnt);
}

static int
mged_pipe_pscale(struct mged_state *s, int mode)
{
    if (s->s_edit->e_inpara > 1) {
	bu_vls_printf(s->s_edit->log_str, "ERROR: only one argument needed\n");
	s->s_edit->e_inpara = 0;
	return TCL_ERROR;
    }

    /* must convert to base units */
    s->s_edit->e_para[0] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[1] *= s->dbip->dbi_local2base;
    s->s_edit->e_para[2] *= s->dbip->dbi_local2base;

    switch (mode) {
	case MENU_PIPE_PT_OD:   /* scale OD of one pipe segment */
	    return menu_pipe_pt_od(s);
	case MENU_PIPE_PT_ID:   /* scale ID of one pipe segment */
	    return menu_pipe_pt_id(s);
	case MENU_PIPE_PT_RADIUS:       /* scale bend radius at selected point */
	    return menu_pipe_pt_radius(s);
	case MENU_PIPE_SCALE_OD:        /* scale entire pipe OD */
	    return menu_pipe_scale_od(s);
	case MENU_PIPE_SCALE_ID:        /* scale entire pipe ID */
	    return menu_pipe_scale_id(s);
	case MENU_PIPE_SCALE_RADIUS:    /* scale entire pipr bend radius */
	    return menu_pipe_scale_radius(s);
    };

    return 0;
}

int
mged_pipe_edit(struct mged_state *s, int edflag)
{
    switch (edflag) {
	case SSCALE:
	    /* scale the solid uniformly about its vertex point */
	    es_pipe_pnt = (struct wdb_pipe_pnt *)NULL; /* Reset es_pipe_pnt */
	    return mged_generic_sscale(s, &s->s_edit->es_int);
	case STRANS:
	    /* translate solid */
	    es_pipe_pnt = (struct wdb_pipe_pnt *)NULL; /* Reset es_pipe_pnt */
	    mged_generic_strans(s, &s->s_edit->es_int);
	    break;
	case SROT:
	    /* rot solid about vertex */
	    es_pipe_pnt = (struct wdb_pipe_pnt *)NULL; /* Reset es_pipe_pnt */
	    mged_generic_srot(s, &s->s_edit->es_int);
	    break;
	case PSCALE:
	    return mged_pipe_pscale(s, s->s_edit->edit_menu);
	case ECMD_PIPE_PICK:
	    ecmd_pipe_pick(s);
	    break;
	case ECMD_PIPE_SPLIT:
	    ecmd_pipe_split(s);
	    break;
	case ECMD_PIPE_PT_MOVE:
	    ecmd_pipe_pt_move(s);
	    break;
	case ECMD_PIPE_PT_ADD:
	    ecmd_pipe_pt_add(s);
	    break;
	case ECMD_PIPE_PT_INS:
	    ecmd_pipe_pt_ins(s);
	    break;
	case ECMD_PIPE_PT_DEL:
	    ecmd_pipe_pt_del(s);
	    break;
    }

    return 0;
}

int
mged_pipe_edit_xy(
	struct mged_state *s,
	int edflag,
	const vect_t mousevec
	)
{
    vect_t pos_view = VINIT_ZERO;       /* Unrotated view space pos */
    vect_t temp = VINIT_ZERO;
    struct rt_db_internal *ip = &s->s_edit->es_int;

    switch (edflag) {
	case SSCALE:
	case PSCALE:
	    mged_generic_sscale_xy(s, mousevec);
	    mged_pipe_edit(s, edflag);
	    return 0;
	case STRANS:
	    mged_generic_strans_xy(&pos_view, s, mousevec);
	    break;
	case ECMD_PIPE_PICK:
	case ECMD_PIPE_SPLIT:
	case ECMD_PIPE_PT_MOVE:
	case ECMD_PIPE_PT_ADD:
	case ECMD_PIPE_PT_INS:
	    MAT4X3PNT(pos_view, view_state->vs_gvp->gv_model2view, s->s_edit->curr_e_axes_pos);
	    pos_view[X] = mousevec[X];
	    pos_view[Y] = mousevec[Y];
	    MAT4X3PNT(temp, view_state->vs_gvp->gv_view2model, pos_view);
	    MAT4X3PNT(s->s_edit->e_mparam, s->s_edit->e_invmat, temp);
	    s->s_edit->e_mvalid = 1;
	    break;
	default:
	    bu_vls_printf(s->s_edit->log_str, "%s: XY edit undefined in solid edit mode %d\n", MGED_OBJ[ip->idb_type].ft_label, edflag);
	    mged_print_result(s, TCL_ERROR);
	    return TCL_ERROR;
    }

    update_edit_absolute_tran(s, pos_view);
    mged_pipe_edit(s, edflag);

    return 0;
}

/*
 * Local Variables:
 * mode: C
 * tab-width: 8
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
