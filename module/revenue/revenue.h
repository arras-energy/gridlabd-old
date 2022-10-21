// File: revenue.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _REVENUE_H

#define _REVENUE_H

#include "gridlabd.h"

#include "globals.h"

#ifdef DLMAIN
#define INIT(X) = X
#define GLOBAL
#else
#define INIT(X)
#define GLOBAL extern
#endif

GLOBAL char1024 billing_library INIT("/usr/local/opt/gridlabd/current/share");
GLOBAL char1024 billing_module INIT("default_billing.py");
GLOBAL char1024 billing_function INIT("compute_bill");
// GLOBAL double lowincome_discount INIT(0);
// GLOBAL double program_credit INIT(0);
// GLOBAL char1024 program_credit_months;
// GLOBAL char1024 summer_season_months;
// GLOBAL char1024 winter_season_months;
// TODO: declare module globals here

#include "tariff.h"
#include "billing.h"

#endif // _REVENUE_H