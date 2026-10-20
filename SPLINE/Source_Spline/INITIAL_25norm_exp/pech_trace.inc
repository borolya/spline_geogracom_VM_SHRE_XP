/*
 * Снимок состояния legacy INITIAL для сравнения с Python.
 *
 * Этот файл подключается ТОЛЬКО из Unit1.cpp после объявлений PLAN, DEFECT,
 * plan, def, n_pts и ndef (после исходной строки 40). Он не является
 * самостоятельной единицей компиляции.
 *
 * Пустой указатель или отсутствующий элемент записывается текстом NaN.
 * В память программы NaN не помещается, поэтому трассировка не меняет расчёт.
 */

static FILE* pech_trace_file = NULL;

static void pech_trace_open(const char* file_name)
{
    pech_trace_file = fopen(file_name, "wt");
    if (pech_trace_file == NULL)
        return;

    fprintf(
        pech_trace_file,
        "# SCALAR: stage\\tSCALAR\\tgroup\\tname\\tindex\\tvalue\n"
    );
    fprintf(
        pech_trace_file,
        "# MASNORM: stage\\tMASNORM\\tnormal\\tshift\\tangle\\tfields...\n"
    );
    fprintf(
        pech_trace_file,
        "# LINE: stage\\tLINE\\tindex\\tfields...\n"
    );
    fflush(pech_trace_file);
}

static void pech_trace_close(void)
{
    if (pech_trace_file == NULL)
        return;

    fflush(pech_trace_file);
    fclose(pech_trace_file);
    pech_trace_file = NULL;
}

static void pech_trace_nan(
    const char* stage,
    const char* group,
    const char* name,
    int index
)
{
    if (pech_trace_file == NULL)
        return;

    fprintf(
        pech_trace_file,
        "%s\tSCALAR\t%s\t%s\t%d\tNaN\n",
        stage, group, name, index
    );
}

static void pech_trace_double(
    const char* stage,
    const char* group,
    const char* name,
    int index,
    double value
)
{
    if (pech_trace_file == NULL)
        return;

    fprintf(
        pech_trace_file,
        "%s\tSCALAR\t%s\t%s\t%d\t%.17g\n",
        stage, group, name, index, value
    );
}

static void pech_trace_int(
    const char* stage,
    const char* group,
    const char* name,
    int index,
    int value
)
{
    if (pech_trace_file == NULL)
        return;

    fprintf(
        pech_trace_file,
        "%s\tSCALAR\t%s\t%s\t%d\t%d\n",
        stage, group, name, index, value
    );
}

static void pech_trace_inf_fields(FILE* file, INF* value)
{
    if (value == NULL) {
        fprintf(
            file,
            "norm=NaN\tntoch=NaN\tnugl=NaN\t"
            "xp=NaN\typ=NaN\tugp=NaN\tsum=NaN\t"
            "R=NaN\tL1=NaN\tL2=NaN\tLk=NaN\t"
            "def1=NaN\tdef2=NaN\tteta=NaN\t"
            "xv=NaN\tyv=NaN\t"
            "xlev=NaN\tylev=NaN\txpr=NaN\typr=NaN"
        );
        return;
    }

    fprintf(
        file,
        "norm=%d\tntoch=%d\tnugl=%d\t"
        "xp=%.17g\typ=%.17g\tugp=%.17g\tsum=%.17g\t"
        "R=%.17g\tL1=%.17g\tL2=%.17g\tLk=%.17g\t"
        "def1=%.17g\tdef2=%.17g\tteta=%.17g\t"
        "xv=%.17g\tyv=%.17g\t"
        "xlev=%.17g\tylev=%.17g\txpr=%.17g\typr=%.17g",
        value->norm,
        value->ntoch,
        value->nugl,
        value->xp,
        value->yp,
        value->ugp,
        value->sum,
        value->R,
        value->L1,
        value->L2,
        value->Lk,
        value->def1,
        value->def2,
        value->teta,
        value->xv,
        value->yv,
        value->xlev,
        value->ylev,
        value->xpr,
        value->ypr
    );
}

#define PECH_TRACE_INT_GLOBAL(variable) \
    pech_trace_int(stage, "global", #variable, -1, variable)

#define PECH_TRACE_DOUBLE_GLOBAL(variable) \
    pech_trace_double(stage, "global", #variable, -1, variable)

static void dump_pech_state(
    const char* stage
)
{
    int i;
    int m;
    int angle_index;

    if (pech_trace_file == NULL)
        return;

    /* Локальные переменные pech передаются явно. */

    /* Все скалярные global-переменные pech и вызываемого поиска. */
    PECH_TRACE_INT_GLOBAL(n);
    PECH_TRACE_INT_GLOBAL(n_pts);
    PECH_TRACE_INT_GLOBAL(nach);
    PECH_TRACE_INT_GLOBAL(kon);
    PECH_TRACE_INT_GLOBAL(inach);
    PECH_TRACE_INT_GLOBAL(ikon);
    PECH_TRACE_INT_GLOBAL(Nnorm);
    PECH_TRACE_INT_GLOBAL(nver);
    PECH_TRACE_INT_GLOBAL(ntoch);
    PECH_TRACE_INT_GLOBAL(nugl);
    PECH_TRACE_INT_GLOBAL(nverp);
    PECH_TRACE_INT_GLOBAL(ntochp);
    PECH_TRACE_INT_GLOBAL(nuglp);
    PECH_TRACE_INT_GLOBAL(nminotkl);
    PECH_TRACE_INT_GLOBAL(npr);
    PECH_TRACE_INT_GLOBAL(nug);
    PECH_TRACE_INT_GLOBAL(pov);
    PECH_TRACE_INT_GLOBAL(pribl);
    PECH_TRACE_INT_GLOBAL(ndef);
    PECH_TRACE_INT_GLOBAL(nkr);
    PECH_TRACE_INT_GLOBAL(nzapf);
    PECH_TRACE_INT_GLOBAL(nzapmax);
    PECH_TRACE_INT_GLOBAL(nzapmax1);
    PECH_TRACE_INT_GLOBAL(nprz);
    PECH_TRACE_INT_GLOBAL(nugz);
    PECH_TRACE_INT_GLOBAL(pr);
    PECH_TRACE_INT_GLOBAL(icorr);
    PECH_TRACE_INT_GLOBAL(ipp);
    PECH_TRACE_INT_GLOBAL(kdL);
    PECH_TRACE_INT_GLOBAL(kdR);

    PECH_TRACE_DOUBLE_GLOBAL(Xmax);
    PECH_TRACE_DOUBLE_GLOBAL(dH);
    PECH_TRACE_DOUBLE_GLOBAL(maxrix);
    PECH_TRACE_DOUBLE_GLOBAL(dx);
    PECH_TRACE_DOUBLE_GLOBAL(dy);
    PECH_TRACE_DOUBLE_GLOBAL(dugl);
    PECH_TRACE_DOUBLE_GLOBAL(xl);
    PECH_TRACE_DOUBLE_GLOBAL(yl);
    PECH_TRACE_DOUBLE_GLOBAL(xp);
    PECH_TRACE_DOUBLE_GLOBAL(yp);
    PECH_TRACE_DOUBLE_GLOBAL(ugl);
    PECH_TRACE_DOUBLE_GLOBAL(ugp);
    PECH_TRACE_DOUBLE_GLOBAL(xv);
    PECH_TRACE_DOUBLE_GLOBAL(yv);
    PECH_TRACE_DOUBLE_GLOBAL(teta);
    PECH_TRACE_DOUBLE_GLOBAL(t1);
    PECH_TRACE_DOUBLE_GLOBAL(t2);
    PECH_TRACE_DOUBLE_GLOBAL(t);
    PECH_TRACE_DOUBLE_GLOBAL(Tmin);
    PECH_TRACE_DOUBLE_GLOBAL(Tmax);
    PECH_TRACE_DOUBLE_GLOBAL(Rmin);
    PECH_TRACE_DOUBLE_GLOBAL(Rmax);
    PECH_TRACE_DOUBLE_GLOBAL(Rmin1);
    PECH_TRACE_DOUBLE_GLOBAL(Bc);
    PECH_TRACE_DOUBLE_GLOBAL(Bcmin);
    PECH_TRACE_DOUBLE_GLOBAL(Bcmax);
    PECH_TRACE_DOUBLE_GLOBAL(Lmin);
    PECH_TRACE_DOUBLE_GLOBAL(Lmax);
    PECH_TRACE_DOUBLE_GLOBAL(lmin);
    PECH_TRACE_DOUBLE_GLOBAL(lmax);
    PECH_TRACE_DOUBLE_GLOBAL(lfirst);
    PECH_TRACE_DOUBLE_GLOBAL(lkmin);
    PECH_TRACE_DOUBLE_GLOBAL(lprmin);
    PECH_TRACE_DOUBLE_GLOBAL(L1);
    PECH_TRACE_DOUBLE_GLOBAL(L2);
    PECH_TRACE_DOUBLE_GLOBAL(Lk);
    PECH_TRACE_DOUBLE_GLOBAL(dL);
    PECH_TRACE_DOUBLE_GLOBAL(dpr);
    PECH_TRACE_DOUBLE_GLOBAL(dug);
    PECH_TRACE_DOUBLE_GLOBAL(def1);
    PECH_TRACE_DOUBLE_GLOBAL(def2);
    PECH_TRACE_DOUBLE_GLOBAL(def1prev);
    PECH_TRACE_DOUBLE_GLOBAL(def2prev);
    PECH_TRACE_DOUBLE_GLOBAL(def1isx);
    PECH_TRACE_DOUBLE_GLOBAL(def2isx);
    PECH_TRACE_DOUBLE_GLOBAL(tetamin);
    PECH_TRACE_DOUBLE_GLOBAL(sum);
    PECH_TRACE_DOUBLE_GLOBAL(sumall);
    PECH_TRACE_DOUBLE_GLOBAL(dtek);
    PECH_TRACE_DOUBLE_GLOBAL(dtek1);
    PECH_TRACE_DOUBLE_GLOBAL(dopt);
    PECH_TRACE_DOUBLE_GLOBAL(d3);
    PECH_TRACE_DOUBLE_GLOBAL(d4);
    PECH_TRACE_DOUBLE_GLOBAL(s);
    PECH_TRACE_DOUBLE_GLOBAL(s1);
    PECH_TRACE_DOUBLE_GLOBAL(s2);
    PECH_TRACE_DOUBLE_GLOBAL(s3);
    PECH_TRACE_DOUBLE_GLOBAL(s4);

    /* Исходные массивы: x имеет n элементов, остальные — n+1. */
    for (i = 0; i < n; i++) {
        if (x == NULL)
            pech_trace_nan(stage, "x", "value", i);
        else
            pech_trace_double(stage, "x", "value", i, x[i]);
    }

    for (i = 0; i <= n; i++) {
        if (xt == NULL)
            pech_trace_nan(stage, "xt", "value", i);
        else
            pech_trace_double(stage, "xt", "value", i, xt[i]);

        if (yt == NULL)
            pech_trace_nan(stage, "yt", "value", i);
        else
            pech_trace_double(stage, "yt", "value", i, yt[i]);

        if (beta == NULL)
            pech_trace_nan(stage, "beta", "value", i);
        else
            pech_trace_double(stage, "beta", "value", i, beta[i]);
    }

    /* Найденные прямые и поворотные области def[]. */
    if (def == NULL || ndef <= 0) {
        pech_trace_nan(stage, "def", "nach", -1);
        pech_trace_nan(stage, "def", "kon", -1);
        pech_trace_nan(stage, "def", "Rdef", -1);
        pech_trace_nan(stage, "def", "d", -1);
        pech_trace_nan(stage, "def", "L", -1);
    }
    else {
        for (i = 0; i < ndef; i++) {
            pech_trace_int(stage, "def", "nach", i, def[i].nach);
            pech_trace_int(stage, "def", "kon", i, def[i].kon);
            pech_trace_double(stage, "def", "Rdef", i, def[i].Rdef);
            pech_trace_double(stage, "def", "d", i, def[i].d);
            pech_trace_double(stage, "def", "L", i, def[i].L);
        }
    }

    /* Временный анализ исходной ломаной plan[]. */
    if (plan == NULL || n_pts <= 0) {
        pech_trace_nan(stage, "plan", "x", -1);
        pech_trace_nan(stage, "plan", "y", -1);
        pech_trace_nan(stage, "plan", "s", -1);
        pech_trace_nan(stage, "plan", "ugl", -1);
        pech_trace_nan(stage, "plan", "sig", -1);
    }
    else {
        for (i = 0; i < n_pts; i++) {
            pech_trace_double(stage, "plan", "x", i, plan[i].x);
            pech_trace_double(stage, "plan", "y", i, plan[i].y);
            pech_trace_double(stage, "plan", "s", i, plan[i].s);
            pech_trace_double(stage, "plan", "ugl", i, plan[i].ugl);
            pech_trace_double(stage, "plan", "sig", i, plan[i].sig);
        }
    }

    /* Полная разреженная таблица состояний masnorm. */
    if (masnorm == NULL || Nnorm <= 0) {
        fprintf(pech_trace_file, "%s\tMASNORM\tNaN\tNaN\tNaN\t", stage);
        pech_trace_inf_fields(pech_trace_file, NULL);
        fprintf(pech_trace_file, "\tpred_norm=NaN\tpred_ntoch=NaN\tpred_nugl=NaN\n");
    }
    else {
        for (i = 0; i < Nnorm; i++) {
            pech_trace_int(stage, "masnorm", "pr", i, masnorm[i].pr);

            if (masnorm[i].point == NULL) {
                fprintf(pech_trace_file, "%s\tMASNORM\t%d\tNaN\tNaN\t", stage, i);
                pech_trace_inf_fields(pech_trace_file, NULL);
                fprintf(pech_trace_file, "\tpred_norm=NaN\tpred_ntoch=NaN\tpred_nugl=NaN\n");
                continue;
            }

            for (m = 0; m <= 2 * npr; m++) {
                if (masnorm[i].point[m].ukp == NULL) {
                    fprintf(pech_trace_file, "%s\tMASNORM\t%d\t%d\tNaN\t", stage, i, m);
                    pech_trace_inf_fields(pech_trace_file, NULL);
                    fprintf(pech_trace_file, "\tpred_norm=NaN\tpred_ntoch=NaN\tpred_nugl=NaN\n");
                    continue;
                }

                for (angle_index = 0; angle_index <= 2 * nug; angle_index++) {
                    ANGLE* angle = &masnorm[i].point[m].ukp[angle_index];
                    INF* predecessor = NULL;

                    if (angle->uk != NULL)
                        predecessor = angle->uk->sv;

                    fprintf(
                        pech_trace_file,
                        "%s\tMASNORM\t%d\t%d\t%d\t",
                        stage, i, m, angle_index
                    );
                    pech_trace_inf_fields(pech_trace_file, angle->sv);

                    if (predecessor == NULL) {
                        fprintf(
                            pech_trace_file,
                            "\tpred_norm=NaN\tpred_ntoch=NaN\tpred_nugl=NaN\n"
                        );
                    }
                    else {
                        fprintf(
                            pech_trace_file,
                            "\tpred_norm=%d\tpred_ntoch=%d\tpred_nugl=%d\n",
                            predecessor->norm,
                            predecessor->ntoch,
                            predecessor->nugl
                        );
                    }
                }
            }
        }
    }

    /* Цепочка, восстановленная zapline(). */
    if (Line == NULL || nkr <= 0) {
        fprintf(pech_trace_file, "%s\tLINE\tNaN\t", stage);
        pech_trace_inf_fields(pech_trace_file, NULL);
        fprintf(pech_trace_file, "\n");
    }
    else {
        for (i = 0; i < nkr; i++) {
            fprintf(pech_trace_file, "%s\tLINE\t%d\t", stage, i);
            pech_trace_inf_fields(pech_trace_file, Line[i]);
            fprintf(pech_trace_file, "\n");
        }
    }

    fprintf(pech_trace_file, "%s\tEND_SNAPSHOT\n", stage);
    fflush(pech_trace_file);
}

#undef PECH_TRACE_INT_GLOBAL
#undef PECH_TRACE_DOUBLE_GLOBAL

