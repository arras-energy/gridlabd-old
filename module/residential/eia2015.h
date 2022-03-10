// $Id$
//
// This file contains the EIA 2015 enduse daily energy uses 
// The data obtained from EIA Table C5.1A/B
//

static IMPLICITENDUSEDATA eia2015[] = {
	{	"LIGHTS",
		{30, false, {0.5,0.1,0.4}, 0.97, 0.9},
		"type:analog; schedule: residential-lights-default; power: 3.6",
		"residential-lights-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.380; *  1 * 4-9 1-5 0.340; *  2 * 4-9 1-5 0.320; *  3 * 4-9 1-5 0.320;"
		"*  4 * 4-9 1-5 0.320; *  5 * 4-9 1-5 0.350; *  6 * 4-9 1-5 0.410; *  7 * 4-9 1-5 0.450;"
		"*  8 * 4-9 1-5 0.450; *  9 * 4-9 1-5 0.450; * 10 * 4-9 1-5 0.450; * 11 * 4-9 1-5 0.450;"
		"* 12 * 4-9 1-5 0.450; * 13 * 4-9 1-5 0.440; * 14 * 4-9 1-5 0.440; * 15 * 4-9 1-5 0.450;"
		"* 16 * 4-9 1-5 0.470; * 17 * 4-9 1-5 0.510; * 18 * 4-9 1-5 0.540; * 19 * 4-9 1-5 0.560;"
		"* 20 * 4-9 1-5 0.630; * 21 * 4-9 1-5 0.710; * 22 * 4-9 1-5 0.650; * 23 * 4-9 1-5 0.490"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.410; *  1 * 4-9 6-0 0.360; *  2 * 4-9 6-0 0.330; *  3 * 4-9 6-0 0.320;"
		"*  4 * 4-9 6-0 0.320; *  5 * 4-9 6-0 0.320; *  6 * 4-9 6-0 0.340; *  7 * 4-9 6-0 0.390;"
		"*  8 * 4-9 6-0 0.440; *  9 * 4-9 6-0 0.470; * 10 * 4-9 6-0 0.470; * 11 * 4-9 6-0 0.470;"
		"* 12 * 4-9 6-0 0.470; * 13 * 4-9 6-0 0.460; * 14 * 4-9 6-0 0.460; * 15 * 4-9 6-0 0.460;"
		"* 16 * 4-9 6-0 0.470; * 17 * 4-9 6-0 0.490; * 18 * 4-9 6-0 0.520; * 19 * 4-9 6-0 0.540;"
		"* 20 * 4-9 6-0 0.610; * 21 * 4-9 6-0 0.680; * 22 * 4-9 6-0 0.630; * 23 * 4-9 6-0 0.500"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.4200; *  1 * 10-3 1-5 0.3800; *  2 * 10-3 1-5 0.3700; *  3 * 10-3 1-5 0.3600;"
		"*  4 * 10-3 1-5 0.3700; *  5 * 10-3 1-5 0.4200; *  6 * 10-3 1-5 0.5800; *  7 * 10-3 1-5 0.6900;"
		"*  8 * 10-3 1-5 0.6100; *  9 * 10-3 1-5 0.5600; * 10 * 10-3 1-5 0.5300; * 11 * 10-3 1-5 0.5100;"
		"* 12 * 10-3 1-5 0.4900; * 13 * 10-3 1-5 0.4700; * 14 * 10-3 1-5 0.4700; * 15 * 10-3 1-5 0.5100;"
		"* 16 * 10-3 1-5 0.6300; * 17 * 10-3 1-5 0.8400; * 18 * 10-3 1-5 0.9700; * 19 * 10-3 1-5 0.9800;"
		"* 20 * 10-3 1-5 0.9600; * 21 * 10-3 1-5 0.8900; * 22 * 10-3 1-5 0.7400; * 23 * 10-3 1-5 0.5500"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.4900; *  1 * 10-3 6-0 0.4200; *  2 * 10-3 6-0 0.3800; *  3 * 10-3 6-0 0.3800;"
		"*  4 * 10-3 6-0 0.3700; *  5 * 10-3 6-0 0.3800; *  6 * 10-3 6-0 0.4300; *  7 * 10-3 6-0 0.5100;"
		"*  8 * 10-3 6-0 0.6000; *  9 * 10-3 6-0 0.6300; * 10 * 10-3 6-0 0.6300; * 11 * 10-3 6-0 0.6100;"
		"* 12 * 10-3 6-0 0.6000; * 13 * 10-3 6-0 0.5900; * 14 * 10-3 6-0 0.5900; * 15 * 10-3 6-0 0.6100;"
		"* 16 * 10-3 6-0 0.7100; * 17 * 10-3 6-0 0.8800; * 18 * 10-3 6-0 0.9600; * 19 * 10-3 6-0 0.9700;"
		"* 20 * 10-3 6-0 0.9400; * 21 * 10-3 6-0 0.8800; * 22 * 10-3 6-0 0.7600; * 23 * 10-3 6-0 0.5800"
		"}"
		},
		// Plugs (source: ELCAP lit-sp.dat)
	{	"PLUGS",
		{30, false, {0.0,0.0,1.0}, 0.90, 0.9},
		"type:analog; schedule: residential-plugs-default; power: 4.4",
		"residential-plugs-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.380; *  1 * 4-9 1-5 0.340; *  2 * 4-9 1-5 0.320; *  3 * 4-9 1-5 0.320;"
		"*  4 * 4-9 1-5 0.320; *  5 * 4-9 1-5 0.350; *  6 * 4-9 1-5 0.410; *  7 * 4-9 1-5 0.450;"
		"*  8 * 4-9 1-5 0.450; *  9 * 4-9 1-5 0.450; * 10 * 4-9 1-5 0.450; * 11 * 4-9 1-5 0.450;"
		"* 12 * 4-9 1-5 0.450; * 13 * 4-9 1-5 0.440; * 14 * 4-9 1-5 0.440; * 15 * 4-9 1-5 0.450;"
		"* 16 * 4-9 1-5 0.470; * 17 * 4-9 1-5 0.510; * 18 * 4-9 1-5 0.540; * 19 * 4-9 1-5 0.560;"
		"* 20 * 4-9 1-5 0.630; * 21 * 4-9 1-5 0.710; * 22 * 4-9 1-5 0.650; * 23 * 4-9 1-5 0.490"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.410; *  1 * 4-9 6-0 0.360; *  2 * 4-9 6-0 0.330; *  3 * 4-9 6-0 0.320;"
		"*  4 * 4-9 6-0 0.320; *  5 * 4-9 6-0 0.320; *  6 * 4-9 6-0 0.340; *  7 * 4-9 6-0 0.390;"
		"*  8 * 4-9 6-0 0.440; *  9 * 4-9 6-0 0.470; * 10 * 4-9 6-0 0.470; * 11 * 4-9 6-0 0.470;"
		"* 12 * 4-9 6-0 0.470; * 13 * 4-9 6-0 0.460; * 14 * 4-9 6-0 0.460; * 15 * 4-9 6-0 0.460;"
		"* 16 * 4-9 6-0 0.470; * 17 * 4-9 6-0 0.490; * 18 * 4-9 6-0 0.520; * 19 * 4-9 6-0 0.540;"
		"* 20 * 4-9 6-0 0.610; * 21 * 4-9 6-0 0.680; * 22 * 4-9 6-0 0.630; * 23 * 4-9 6-0 0.500"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.4200; *  1 * 10-3 1-5 0.3800; *  2 * 10-3 1-5 0.3700; *  3 * 10-3 1-5 0.3600;"
		"*  4 * 10-3 1-5 0.3700; *  5 * 10-3 1-5 0.4200; *  6 * 10-3 1-5 0.5800; *  7 * 10-3 1-5 0.6900;"
		"*  8 * 10-3 1-5 0.6100; *  9 * 10-3 1-5 0.5600; * 10 * 10-3 1-5 0.5300; * 11 * 10-3 1-5 0.5100;"
		"* 12 * 10-3 1-5 0.4900; * 13 * 10-3 1-5 0.4700; * 14 * 10-3 1-5 0.4700; * 15 * 10-3 1-5 0.5100;"
		"* 16 * 10-3 1-5 0.6300; * 17 * 10-3 1-5 0.8400; * 18 * 10-3 1-5 0.9700; * 19 * 10-3 1-5 0.9800;"
		"* 20 * 10-3 1-5 0.9600; * 21 * 10-3 1-5 0.8900; * 22 * 10-3 1-5 0.7400; * 23 * 10-3 1-5 0.5500"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.4900; *  1 * 10-3 6-0 0.4200; *  2 * 10-3 6-0 0.3800; *  3 * 10-3 6-0 0.3800;"
		"*  4 * 10-3 6-0 0.3700; *  5 * 10-3 6-0 0.3800; *  6 * 10-3 6-0 0.4300; *  7 * 10-3 6-0 0.5100;"
		"*  8 * 10-3 6-0 0.6000; *  9 * 10-3 6-0 0.6300; * 10 * 10-3 6-0 0.6300; * 11 * 10-3 6-0 0.6100;"
		"* 12 * 10-3 6-0 0.6000; * 13 * 10-3 6-0 0.5900; * 14 * 10-3 6-0 0.5900; * 15 * 10-3 6-0 0.6100;"
		"* 16 * 10-3 6-0 0.7100; * 17 * 10-3 6-0 0.8800; * 18 * 10-3 6-0 0.9600; * 19 * 10-3 6-0 0.9700;"
		"* 20 * 10-3 6-0 0.9400; * 21 * 10-3 6-0 0.8800; * 22 * 10-3 6-0 0.7600; * 23 * 10-3 6-0 0.5800"
		"}"
		},

	{   "CLOTHESWASHER",
		{20, false, {0.0,0.0,1.0}, 0.9, 1.0},
		"type:analog; schedule: residential-clotheswasher-default; power: 0.16",
		"residential-clotheswasher-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.0029; *  1 * 4-9 1-5 0.0019; *  2 * 4-9 1-5 0.0014; *  3 * 4-9 1-5 0.0013;"
		"*  4 * 4-9 1-5 0.0018; *  5 * 4-9 1-5 0.0026; *  6 * 4-9 1-5 0.0055; *  7 * 4-9 1-5 0.0126;"
		"*  8 * 4-9 1-5 0.0181; *  9 * 4-9 1-5 0.0208; * 10 * 4-9 1-5 0.0229; * 11 * 4-9 1-5 0.0216;"
		"* 12 * 4-9 1-5 0.0193; * 13 * 4-9 1-5 0.0170; * 14 * 4-9 1-5 0.0145; * 15 * 4-9 1-5 0.0135;"
		"* 16 * 4-9 1-5 0.0135; * 17 * 4-9 1-5 0.0142; * 18 * 4-9 1-5 0.0145; * 19 * 4-9 1-5 0.0148;"
		"* 20 * 4-9 1-5 0.0146; * 21 * 4-9 1-5 0.0141; * 22 * 4-9 1-5 0.0110; * 23 * 4-9 1-5 0.0062"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.0031; *  1 * 4-9 6-0 0.0019; *  2 * 4-9 6-0 0.0013; *  3 * 4-9 6-0 0.0012;"
		"*  4 * 4-9 6-0 0.0012; *  5 * 4-9 6-0 0.0016; *  6 * 4-9 6-0 0.0027; *  7 * 4-9 6-0 0.0066;"
		"*  8 * 4-9 6-0 0.0157; *  9 * 4-9 6-0 0.0220; * 10 * 4-9 6-0 0.0258; * 11 * 4-9 6-0 0.0251;"
		"* 12 * 4-9 6-0 0.0231; * 13 * 4-9 6-0 0.0217; * 14 * 4-9 6-0 0.0186; * 15 * 4-9 6-0 0.0157;"
		"* 16 * 4-9 6-0 0.0156; * 17 * 4-9 6-0 0.0151; * 18 * 4-9 6-0 0.0147; * 19 * 4-9 6-0 0.0150;"
		"* 20 * 4-9 6-0 0.0156; * 21 * 4-9 6-0 0.0148; * 22 * 4-9 6-0 0.0106; * 23 * 4-9 6-0 0.0065"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.0036; *  1 * 10-3 1-5 0.0024; *  2 * 10-3 1-5 0.0020; *  3 * 10-3 1-5 0.0019;"
		"*  4 * 10-3 1-5 0.0026; *  5 * 10-3 1-5 0.0040; *  6 * 10-3 1-5 0.0062; *  7 * 10-3 1-5 0.0118;"
		"*  8 * 10-3 1-5 0.0177; *  9 * 10-3 1-5 0.0211; * 10 * 10-3 1-5 0.0215; * 11 * 10-3 1-5 0.0203;"
		"* 12 * 10-3 1-5 0.0176; * 13 * 10-3 1-5 0.0155; * 14 * 10-3 1-5 0.0133; * 15 * 10-3 1-5 0.0130;"
		"* 16 * 10-3 1-5 0.0145; * 17 * 10-3 1-5 0.0159; * 18 * 10-3 1-5 0.0166; * 19 * 10-3 1-5 0.0164;"
		"* 20 * 10-3 1-5 0.0154; * 21 * 10-3 1-5 0.0149; * 22 * 10-3 1-5 0.0110; * 23 * 10-3 1-5 0.0065"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.0044; *  1 * 10-3 6-0 0.0030; *  2 * 10-3 6-0 0.0022; *  3 * 10-3 6-0 0.0020;"
		"*  4 * 10-3 6-0 0.0021; *  5 * 10-3 6-0 0.0021; *  6 * 10-3 6-0 0.0030; *  7 * 10-3 6-0 0.0067;"
		"*  8 * 10-3 6-0 0.0145; *  9 * 10-3 6-0 0.0244; * 10 * 10-3 6-0 0.0310; * 11 * 10-3 6-0 0.0323;"
		"* 12 * 10-3 6-0 0.0308; * 13 * 10-3 6-0 0.0285; * 14 * 10-3 6-0 0.0251; * 15 * 10-3 6-0 0.0224;"
		"* 16 * 10-3 6-0 0.0215; * 17 * 10-3 6-0 0.0203; * 18 * 10-3 6-0 0.0194; * 19 * 10-3 6-0 0.0188;"
		"* 20 * 10-3 6-0 0.0180; * 21 * 10-3 6-0 0.0151; * 22 * 10-3 6-0 0.0122; * 23 * 10-3 6-0 0.0073"
		"}"
		},

	{   "WATERHEATER",
		{30, true, {0.0,0.0,1.0}, 1.0, 0.56},
		"type:analog; schedule: residential-waterheater-default; power: 4.0",
		"residential-waterheater-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.21; *  1 * 4-9 1-5 0.16; *  2 * 4-9 1-5 0.13; *  3 * 4-9 1-5 0.12;"
		"*  4 * 4-9 1-5 0.15; *  5 * 4-9 1-5 0.26; *  6 * 4-9 1-5 0.51; *  7 * 4-9 1-5 0.76;"
		"*  8 * 4-9 1-5 0.77; *  9 * 4-9 1-5 0.76; * 10 * 4-9 1-5 0.71; * 11 * 4-9 1-5 0.61;"
		"* 12 * 4-9 1-5 0.54; * 13 * 4-9 1-5 0.49; * 14 * 4-9 1-5 0.43; * 15 * 4-9 1-5 0.41;"
		"* 16 * 4-9 1-5 0.43; * 17 * 4-9 1-5 0.52; * 18 * 4-9 1-5 0.60; * 19 * 4-9 1-5 0.60;"
		"* 20 * 4-9 1-5 0.59; * 21 * 4-9 1-5 0.60; * 22 * 4-9 1-5 0.55; * 23 * 4-9 1-5 0.37"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.23; *  1 * 4-9 6-0 0.17; *  2 * 4-9 6-0 0.14; *  3 * 4-9 6-0 0.13;"
		"*  4 * 4-9 6-0 0.13; *  5 * 4-9 6-0 0.17; *  6 * 4-9 6-0 0.26; *  7 * 4-9 6-0 0.45;"
		"*  8 * 4-9 6-0 0.69; *  9 * 4-9 6-0 0.85; * 10 * 4-9 6-0 0.84; * 11 * 4-9 6-0 0.76;"
		"* 12 * 4-9 6-0 0.65; * 13 * 4-9 6-0 0.58; * 14 * 4-9 6-0 0.49; * 15 * 4-9 6-0 0.46;"
		"* 16 * 4-9 6-0 0.46; * 17 * 4-9 6-0 0.50; * 18 * 4-9 6-0 0.54; * 19 * 4-9 6-0 0.55;"
		"* 20 * 4-9 6-0 0.56; * 21 * 4-9 6-0 0.56; * 22 * 4-9 6-0 0.49; * 23 * 4-9 6-0 0.38"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.25; *  1 * 10-3 1-5 0.19; *  2 * 10-3 1-5 0.16; *  3 * 10-3 1-5 0.15;"
		"*  4 * 10-3 1-5 0.18; *  5 * 10-3 1-5 0.34; *  6 * 10-3 1-5 0.74; *  7 * 10-3 1-5 1.20;"
		"*  8 * 10-3 1-5 1.10; *  9 * 10-3 1-5 0.94; * 10 * 10-3 1-5 0.82; * 11 * 10-3 1-5 0.71;"
		"* 12 * 10-3 1-5 0.62; * 13 * 10-3 1-5 0.55; * 14 * 10-3 1-5 0.48; * 15 * 10-3 1-5 0.47;"
		"* 16 * 10-3 1-5 0.54; * 17 * 10-3 1-5 0.68; * 18 * 10-3 1-5 0.83; * 19 * 10-3 1-5 0.82;"
		"* 20 * 10-3 1-5 0.74; * 21 * 10-3 1-5 0.68; * 22 * 10-3 1-5 0.57; * 23 * 10-3 1-5 0.40"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.29; *  1 * 10-3 6-0 0.22; *  2 * 10-3 6-0 0.17; *  3 * 10-3 6-0 0.15;"
		"*  4 * 10-3 6-0 0.16; *  5 * 10-3 6-0 0.19; *  6 * 10-3 6-0 0.27; *  7 * 10-3 6-0 0.47;"
		"*  8 * 10-3 6-0 0.82; *  9 * 10-3 6-0 1.08; * 10 * 10-3 6-0 1.15; * 11 * 10-3 6-0 1.08;"
		"* 12 * 10-3 6-0 0.98; * 13 * 10-3 6-0 0.87; * 14 * 10-3 6-0 0.77; * 15 * 10-3 6-0 0.69;"
		"* 16 * 10-3 6-0 0.72; * 17 * 10-3 6-0 0.78; * 18 * 10-3 6-0 0.83; * 19 * 10-3 6-0 0.79;"
		"* 20 * 10-3 6-0 0.72; * 21 * 10-3 6-0 0.64; * 22 * 10-3 6-0 0.53; * 23 * 10-3 6-0 0.43"
		"}"
		},

	{   "REFRIGERATOR",
		{15, false, {0.1,0.0,0.9}, 0.9, 1.0},
		"type:analog; schedule: residential-refrigerator-default; power: 2.4",
		"residential-refrigerator-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.187; *  1 * 4-9 1-5 0.182; *  2 * 4-9 1-5 0.176; *  3 * 4-9 1-5 0.170;"
		"*  4 * 4-9 1-5 0.168; *  5 * 4-9 1-5 0.168; *  6 * 4-9 1-5 0.177; *  7 * 4-9 1-5 0.174;"
		"*  8 * 4-9 1-5 0.177; *  9 * 4-9 1-5 0.180; * 10 * 4-9 1-5 0.180; * 11 * 4-9 1-5 0.183;"
		"* 12 * 4-9 1-5 0.192; * 13 * 4-9 1-5 0.192; * 14 * 4-9 1-5 0.194; * 15 * 4-9 1-5 0.196;"
		"* 16 * 4-9 1-5 0.205; * 17 * 4-9 1-5 0.217; * 18 * 4-9 1-5 0.225; * 19 * 4-9 1-5 0.221;"
		"* 20 * 4-9 1-5 0.216; * 21 * 4-9 1-5 0.214; * 22 * 4-9 1-5 0.207; * 23 * 4-9 1-5 0.195"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.187; *  1 * 4-9 6-0 0.181; *  2 * 4-9 6-0 0.176; *  3 * 4-9 6-0 0.169;"
		"*  4 * 4-9 6-0 0.166; *  5 * 4-9 6-0 0.164; *  6 * 4-9 6-0 0.167; *  7 * 4-9 6-0 0.169;"
		"*  8 * 4-9 6-0 0.180; *  9 * 4-9 6-0 0.184; * 10 * 4-9 6-0 0.187; * 11 * 4-9 6-0 0.187;"
		"* 12 * 4-9 6-0 0.195; * 13 * 4-9 6-0 0.200; * 14 * 4-9 6-0 0.201; * 15 * 4-9 6-0 0.203;"
		"* 16 * 4-9 6-0 0.209; * 17 * 4-9 6-0 0.218; * 18 * 4-9 6-0 0.222; * 19 * 4-9 6-0 0.221;"
		"* 20 * 4-9 6-0 0.217; * 21 * 4-9 6-0 0.216; * 22 * 4-9 6-0 0.207; * 23 * 4-9 6-0 0.196"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.1530; *  1 * 10-3 1-5 0.1500; *  2 * 10-3 1-5 0.1460; *  3 * 10-3 1-5 0.1420;"
		"*  4 * 10-3 1-5 0.1400; *  5 * 10-3 1-5 0.1450; *  6 * 10-3 1-5 0.1520; *  7 * 10-3 1-5 0.1600;"
		"*  8 * 10-3 1-5 0.1580; *  9 * 10-3 1-5 0.1580; * 10 * 10-3 1-5 0.1560; * 11 * 10-3 1-5 0.1560;"
		"* 12 * 10-3 1-5 0.1630; * 13 * 10-3 1-5 0.1620; * 14 * 10-3 1-5 0.1590; * 15 * 10-3 1-5 0.1620;"
		"* 16 * 10-3 1-5 0.1690; * 17 * 10-3 1-5 0.1850; * 18 * 10-3 1-5 0.1920; * 19 * 10-3 1-5 0.1820;"
		"* 20 * 10-3 1-5 0.1800; * 21 * 10-3 1-5 0.1760; * 22 * 10-3 1-5 0.1670; * 23 * 10-3 1-5 0.1590"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.1560; *  1 * 10-3 6-0 0.1520; *  2 * 10-3 6-0 0.1470; *  3 * 10-3 6-0 0.1430;"
		"*  4 * 10-3 6-0 0.1420; *  5 * 10-3 6-0 0.1430; *  6 * 10-3 6-0 0.1430; *  7 * 10-3 6-0 0.1500;"
		"*  8 * 10-3 6-0 0.1610; *  9 * 10-3 6-0 0.1690; * 10 * 10-3 6-0 0.1670; * 11 * 10-3 6-0 0.1660;"
		"* 12 * 10-3 6-0 0.1740; * 13 * 10-3 6-0 0.1760; * 14 * 10-3 6-0 0.1740; * 15 * 10-3 6-0 0.1750;"
		"* 16 * 10-3 6-0 0.1790; * 17 * 10-3 6-0 0.1910; * 18 * 10-3 6-0 0.1930; * 19 * 10-3 6-0 0.1870;"
		"* 20 * 10-3 6-0 0.1840; * 21 * 10-3 6-0 0.1780; * 22 * 10-3 6-0 0.1700; * 23 * 10-3 6-0 0.1600"
		"}"
		},

	{   "DRYER",
		{30, true, {0.9,0.0,0.1}, 0.99, 0.15},
		"type:analog; schedule: residential-dryer-default; power: 1.6",
		"residential-dryer-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.036; *  1 * 4-9 1-5 0.013; *  2 * 4-9 1-5 0.007; *  3 * 4-9 1-5 0.005;"
		"*  4 * 4-9 1-5 0.005; *  5 * 4-9 1-5 0.017; *  6 * 4-9 1-5 0.048; *  7 * 4-9 1-5 0.085;"
		"*  8 * 4-9 1-5 0.115; *  9 * 4-9 1-5 0.156; * 10 * 4-9 1-5 0.179; * 11 * 4-9 1-5 0.185;"
		"* 12 * 4-9 1-5 0.172; * 13 * 4-9 1-5 0.162; * 14 * 4-9 1-5 0.145; * 15 * 4-9 1-5 0.136;"
		"* 16 * 4-9 1-5 0.133; * 17 * 4-9 1-5 0.134; * 18 * 4-9 1-5 0.127; * 19 * 4-9 1-5 0.130;"
		"* 20 * 4-9 1-5 0.141; * 21 * 4-9 1-5 0.154; * 22 * 4-9 1-5 0.138; * 23 * 4-9 1-5 0.083"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.041; *  1 * 4-9 6-0 0.017; *  2 * 4-9 6-0 0.008; *  3 * 4-9 6-0 0.005;"
		"*  4 * 4-9 6-0 0.005; *  5 * 4-9 6-0 0.007; *  6 * 4-9 6-0 0.018; *  7 * 4-9 6-0 0.047;"
		"*  8 * 4-9 6-0 0.100; *  9 * 4-9 6-0 0.168; * 10 * 4-9 6-0 0.205; * 11 * 4-9 6-0 0.220;"
		"* 12 * 4-9 6-0 0.211; * 13 * 4-9 6-0 0.210; * 14 * 4-9 6-0 0.188; * 15 * 4-9 6-0 0.168;"
		"* 16 * 4-9 6-0 0.154; * 17 * 4-9 6-0 0.146; * 18 * 4-9 6-0 0.138; * 19 * 4-9 6-0 0.137;"
		"* 20 * 4-9 6-0 0.144; * 21 * 4-9 6-0 0.155; * 22 * 4-9 6-0 0.131; * 23 * 4-9 6-0 0.081"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.0360; *  1 * 10-3 1-5 0.0160; *  2 * 10-3 1-5 0.0100; *  3 * 10-3 1-5 0.0070;"
		"*  4 * 10-3 1-5 0.0090; *  5 * 10-3 1-5 0.0230; *  6 * 10-3 1-5 0.0610; *  7 * 10-3 1-5 0.1030;"
		"*  8 * 10-3 1-5 0.1320; *  9 * 10-3 1-5 0.1750; * 10 * 10-3 1-5 0.2050; * 11 * 10-3 1-5 0.2130;"
		"* 12 * 10-3 1-5 0.1940; * 13 * 10-3 1-5 0.1770; * 14 * 10-3 1-5 0.1610; * 15 * 10-3 1-5 0.1560;"
		"* 16 * 10-3 1-5 0.1640; * 17 * 10-3 1-5 0.1710; * 18 * 10-3 1-5 0.1610; * 19 * 10-3 1-5 0.1590;"
		"* 20 * 10-3 1-5 0.1670; * 21 * 10-3 1-5 0.1690; * 22 * 10-3 1-5 0.1380; * 23 * 10-3 1-5 0.0820"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.0390; *  1 * 10-3 6-0 0.0190; *  2 * 10-3 6-0 0.0110; *  3 * 10-3 6-0 0.0070;"
		"*  4 * 10-3 6-0 0.0080; *  5 * 10-3 6-0 0.0090; *  6 * 10-3 6-0 0.0160; *  7 * 10-3 6-0 0.0430;"
		"*  8 * 10-3 6-0 0.1010; *  9 * 10-3 6-0 0.1810; * 10 * 10-3 6-0 0.2640; * 11 * 10-3 6-0 0.3050;"
		"* 12 * 10-3 6-0 0.3110; * 13 * 10-3 6-0 0.3060; * 14 * 10-3 6-0 0.2850; * 15 * 10-3 6-0 0.2700;"
		"* 16 * 10-3 6-0 0.2600; * 17 * 10-3 6-0 0.2450; * 18 * 10-3 6-0 0.2200; * 19 * 10-3 6-0 0.1980;"
		"* 20 * 10-3 6-0 0.1880; * 21 * 10-3 6-0 0.1790; * 22 * 10-3 6-0 0.1480; * 23 * 10-3 6-0 0.0930"
		"}"
		},

	{   "FREEZER",
		{15, false, {0.1,0.0,0.9}, 0.9, 1.0},
		"type:analog; schedule: residential-freezer-default; power: 0.64",
		"residential-freezer-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.210; *  1 * 4-9 1-5 0.213; *  2 * 4-9 1-5 0.208; *  3 * 4-9 1-5 0.202;"
		"*  4 * 4-9 1-5 0.203; *  5 * 4-9 1-5 0.198; *  6 * 4-9 1-5 0.190; *  7 * 4-9 1-5 0.186;"
		"*  8 * 4-9 1-5 0.189; *  9 * 4-9 1-5 0.194; * 10 * 4-9 1-5 0.199; * 11 * 4-9 1-5 0.202;"
		"* 12 * 4-9 1-5 0.211; * 13 * 4-9 1-5 0.214; * 14 * 4-9 1-5 0.219; * 15 * 4-9 1-5 0.222;"
		"* 16 * 4-9 1-5 0.230; * 17 * 4-9 1-5 0.228; * 18 * 4-9 1-5 0.229; * 19 * 4-9 1-5 0.223;"
		"* 20 * 4-9 1-5 0.224; * 21 * 4-9 1-5 0.223; * 22 * 4-9 1-5 0.218; * 23 * 4-9 1-5 0.214"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.203; *  1 * 4-9 6-0 0.202; *  2 * 4-9 6-0 0.202; *  3 * 4-9 6-0 0.193;"
		"*  4 * 4-9 6-0 0.198; *  5 * 4-9 6-0 0.195; *  6 * 4-9 6-0 0.191; *  7 * 4-9 6-0 0.183;"
		"*  8 * 4-9 6-0 0.184; *  9 * 4-9 6-0 0.192; * 10 * 4-9 6-0 0.197; * 11 * 4-9 6-0 0.202;"
		"* 12 * 4-9 6-0 0.208; * 13 * 4-9 6-0 0.219; * 14 * 4-9 6-0 0.219; * 15 * 4-9 6-0 0.225;"
		"* 16 * 4-9 6-0 0.225; * 17 * 4-9 6-0 0.225; * 18 * 4-9 6-0 0.223; * 19 * 4-9 6-0 0.219;"
		"* 20 * 4-9 6-0 0.221; * 21 * 4-9 6-0 0.220; * 22 * 4-9 6-0 0.215; * 23 * 4-9 6-0 0.209"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.149; *  1 * 10-3 1-5 0.148; *  2 * 10-3 1-5 0.145; *  3 * 10-3 1-5 0.144;"
		"*  4 * 10-3 1-5 0.143; *  5 * 10-3 1-5 0.140; *  6 * 10-3 1-5 0.138; *  7 * 10-3 1-5 0.138;"
		"*  8 * 10-3 1-5 0.140; *  9 * 10-3 1-5 0.141; * 10 * 10-3 1-5 0.142; * 11 * 10-3 1-5 0.147;"
		"* 12 * 10-3 1-5 0.153; * 13 * 10-3 1-5 0.154; * 14 * 10-3 1-5 0.152; * 15 * 10-3 1-5 0.151;"
		"* 16 * 10-3 1-5 0.161; * 17 * 10-3 1-5 0.174; * 18 * 10-3 1-5 0.176; * 19 * 10-3 1-5 0.176;"
		"* 20 * 10-3 1-5 0.175; * 21 * 10-3 1-5 0.169; * 22 * 10-3 1-5 0.160; * 23 * 10-3 1-5 0.153"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.155; *  1 * 10-3 6-0 0.150; *  2 * 10-3 6-0 0.143; *  3 * 10-3 6-0 0.141;"
		"*  4 * 10-3 6-0 0.141; *  5 * 10-3 6-0 0.139; *  6 * 10-3 6-0 0.138; *  7 * 10-3 6-0 0.139;"
		"*  8 * 10-3 6-0 0.142; *  9 * 10-3 6-0 0.142; * 10 * 10-3 6-0 0.145; * 11 * 10-3 6-0 0.153;"
		"* 12 * 10-3 6-0 0.161; * 13 * 10-3 6-0 0.162; * 14 * 10-3 6-0 0.160; * 15 * 10-3 6-0 0.161;"
		"* 16 * 10-3 6-0 0.165; * 17 * 10-3 6-0 0.177; * 18 * 10-3 6-0 0.179; * 19 * 10-3 6-0 0.177;"
		"* 20 * 10-3 6-0 0.171; * 21 * 10-3 6-0 0.168; * 22 * 10-3 6-0 0.160; * 23 * 10-3 6-0 0.151"
		"}"
		},
	{   "DISHWASHER",
		{15, false, {0.8,0,0.2}, 0.98, 1.0},
		"type:analog; schedule: residential-dishwasher-default; power: 0.20",
		"residential-dishwasher-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.0068; *  1 * 4-9 1-5 0.0029; *  2 * 4-9 1-5 0.0016; *  3 * 4-9 1-5 0.0013;"
		"*  4 * 4-9 1-5 0.0012; *  5 * 4-9 1-5 0.0037; *  6 * 4-9 1-5 0.0075; *  7 * 4-9 1-5 0.0129;"
		"*  8 * 4-9 1-5 0.0180; *  9 * 4-9 1-5 0.0177; * 10 * 4-9 1-5 0.0144; * 11 * 4-9 1-5 0.0113;"
		"* 12 * 4-9 1-5 0.0116; * 13 * 4-9 1-5 0.0128; * 14 * 4-9 1-5 0.0109; * 15 * 4-9 1-5 0.0105;"
		"* 16 * 4-9 1-5 0.0124; * 17 * 4-9 1-5 0.0156; * 18 * 4-9 1-5 0.0278; * 19 * 4-9 1-5 0.0343;"
		"* 20 * 4-9 1-5 0.0279; * 21 * 4-9 1-5 0.0234; * 22 * 4-9 1-5 0.0194; * 23 * 4-9 1-5 0.0131"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.0093; *  1 * 4-9 6-0 0.0045; *  2 * 4-9 6-0 0.0021; *  3 * 4-9 6-0 0.0015;"
		"*  4 * 4-9 6-0 0.0013; *  5 * 4-9 6-0 0.0015; *  6 * 4-9 6-0 0.0026; *  7 * 4-9 6-0 0.0067;"
		"*  8 * 4-9 6-0 0.0142; *  9 * 4-9 6-0 0.0221; * 10 * 4-9 6-0 0.0259; * 11 * 4-9 6-0 0.0238;"
		"* 12 * 4-9 6-0 0.0214; * 13 * 4-9 6-0 0.0214; * 14 * 4-9 6-0 0.0188; * 15 * 4-9 6-0 0.0169;"
		"* 16 * 4-9 6-0 0.0156; * 17 * 4-9 6-0 0.0166; * 18 * 4-9 6-0 0.0249; * 19 * 4-9 6-0 0.0298;"
		"* 20 * 4-9 6-0 0.0267; * 21 * 4-9 6-0 0.0221; * 22 * 4-9 6-0 0.0174; * 23 * 4-9 6-0 0.0145"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.0068; *  1 * 10-3 1-5 0.0029; *  2 * 10-3 1-5 0.0016; *  3 * 10-3 1-5 0.0013;"
		"*  4 * 10-3 1-5 0.0012; *  5 * 10-3 1-5 0.0037; *  6 * 10-3 1-5 0.0075; *  7 * 10-3 1-5 0.0129;"
		"*  8 * 10-3 1-5 0.0180; *  9 * 10-3 1-5 0.0177; * 10 * 10-3 1-5 0.0144; * 11 * 10-3 1-5 0.0113;"
		"* 12 * 10-3 1-5 0.0116; * 13 * 10-3 1-5 0.0128; * 14 * 10-3 1-5 0.0109; * 15 * 10-3 1-5 0.0105;"
		"* 16 * 10-3 1-5 0.0124; * 17 * 10-3 1-5 0.0156; * 18 * 10-3 1-5 0.0278; * 19 * 10-3 1-5 0.0343;"
		"* 20 * 10-3 1-5 0.0279; * 21 * 10-3 1-5 0.0234; * 22 * 10-3 1-5 0.0194; * 23 * 10-3 1-5 0.0131"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.0093; *  1 * 10-3 6-0 0.0045; *  2 * 10-3 6-0 0.0021; *  3 * 10-3 6-0 0.0015;"
		"*  4 * 10-3 6-0 0.0013; *  5 * 10-3 6-0 0.0015; *  6 * 10-3 6-0 0.0026; *  7 * 10-3 6-0 0.0067;"
		"*  8 * 10-3 6-0 0.0142; *  9 * 10-3 6-0 0.0221; * 10 * 10-3 6-0 0.0259; * 11 * 10-3 6-0 0.0238;"
		"* 12 * 10-3 6-0 0.0214; * 13 * 10-3 6-0 0.0214; * 14 * 10-3 6-0 0.0188; * 15 * 10-3 6-0 0.0169;"
		"* 16 * 10-3 6-0 0.0156; * 17 * 10-3 6-0 0.0166; * 18 * 10-3 6-0 0.0249; * 19 * 10-3 6-0 0.0298;"
		"* 20 * 10-3 6-0 0.0267; * 21 * 10-3 6-0 0.0221; * 22 * 10-3 6-0 0.0174; * 23 * 10-3 6-0 0.0145"
		"}"
	},
	{   "RANGE",
		{40, true, {1,0,0}, 0.85, 0.8},
		"type:analog; schedule: residential-range-default; power: 0.5",
		"residential-range-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.009; *  1 * 4-9 1-5 0.008; *  2 * 4-9 1-5 0.007; *  3 * 4-9 1-5 0.007;"
		"*  4 * 4-9 1-5 0.008; *  5 * 4-9 1-5 0.012; *  6 * 4-9 1-5 0.025; *  7 * 4-9 1-5 0.040;"
		"*  8 * 4-9 1-5 0.044; *  9 * 4-9 1-5 0.042; * 10 * 4-9 1-5 0.042; * 11 * 4-9 1-5 0.053;"
		"* 12 * 4-9 1-5 0.057; * 13 * 4-9 1-5 0.046; * 14 * 4-9 1-5 0.044; * 15 * 4-9 1-5 0.053;"
		"* 16 * 4-9 1-5 0.094; * 17 * 4-9 1-5 0.168; * 18 * 4-9 1-5 0.148; * 19 * 4-9 1-5 0.086;"
		"* 20 * 4-9 1-5 0.053; * 21 * 4-9 1-5 0.038; * 22 * 4-9 1-5 0.023; * 23 * 4-9 1-5 0.013"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.009; *  1 * 4-9 6-0 0.007; *  2 * 4-9 6-0 0.007; *  3 * 4-9 6-0 0.007;"
		"*  4 * 4-9 6-0 0.007; *  5 * 4-9 6-0 0.009; *  6 * 4-9 6-0 0.017; *  7 * 4-9 6-0 0.038;"
		"*  8 * 4-9 6-0 0.060; *  9 * 4-9 6-0 0.068; * 10 * 4-9 6-0 0.065; * 11 * 4-9 6-0 0.067;"
		"* 12 * 4-9 6-0 0.076; * 13 * 4-9 6-0 0.066; * 14 * 4-9 6-0 0.061; * 15 * 4-9 6-0 0.067;"
		"* 16 * 4-9 6-0 0.091; * 17 * 4-9 6-0 0.134; * 18 * 4-9 6-0 0.121; * 19 * 4-9 6-0 0.080;"
		"* 20 * 4-9 6-0 0.052; * 21 * 4-9 6-0 0.035; * 22 * 4-9 6-0 0.022; * 23 * 4-9 6-0 0.011"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.010; *  1 * 10-3 1-5 0.009; *  2 * 10-3 1-5 0.009; *  3 * 10-3 1-5 0.009;"
		"*  4 * 10-3 1-5 0.009; *  5 * 10-3 1-5 0.016; *  6 * 10-3 1-5 0.032; *  7 * 10-3 1-5 0.050;"
		"*  8 * 10-3 1-5 0.045; *  9 * 10-3 1-5 0.043; * 10 * 10-3 1-5 0.045; * 11 * 10-3 1-5 0.059;"
		"* 12 * 10-3 1-5 0.063; * 13 * 10-3 1-5 0.053; * 14 * 10-3 1-5 0.052; * 15 * 10-3 1-5 0.072;"
		"* 16 * 10-3 1-5 0.138; * 17 * 10-3 1-5 0.242; * 18 * 10-3 1-5 0.182; * 19 * 10-3 1-5 0.088;"
		"* 20 * 10-3 1-5 0.051; * 21 * 10-3 1-5 0.034; * 22 * 10-3 1-5 0.022; * 23 * 10-3 1-5 0.014"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.013; *  1 * 10-3 6-0 0.010; *  2 * 10-3 6-0 0.010; *  3 * 10-3 6-0 0.010;"
		"*  4 * 10-3 6-0 0.010; *  5 * 10-3 6-0 0.012; *  6 * 10-3 6-0 0.018; *  7 * 10-3 6-0 0.040;"
		"*  8 * 10-3 6-0 0.073; *  9 * 10-3 6-0 0.094; * 10 * 10-3 6-0 0.091; * 11 * 10-3 6-0 0.100;"
		"* 12 * 10-3 6-0 0.117; * 13 * 10-3 6-0 0.109; * 14 * 10-3 6-0 0.100; * 15 * 10-3 6-0 0.108;"
		"* 16 * 10-3 6-0 0.153;	* 17 * 10-3 6-0 0.215; * 18 * 10-3 6-0 0.161; * 19 * 10-3 6-0 0.085;"
		"* 20 * 10-3 6-0 0.050;	* 21 * 10-3 6-0 0.033; * 22 * 10-3 6-0 0.022; * 23 * 10-3 6-0 0.014"
		"}"
	},
	{   "MICROWAVE",
		{15, false, {0,0,1}, 0.7, 0.8},
		"type:analog; schedule: residential-microwave-default; power: 0.35",
		"residential-microwave-default",
		"normal; weekday-summer {"
		"*  0 * 4-9 1-5 0.009; *  1 * 4-9 1-5 0.008; *  2 * 4-9 1-5 0.007; *  3 * 4-9 1-5 0.007;"
		"*  4 * 4-9 1-5 0.008; *  5 * 4-9 1-5 0.012; *  6 * 4-9 1-5 0.025; *  7 * 4-9 1-5 0.040;"
		"*  8 * 4-9 1-5 0.044; *  9 * 4-9 1-5 0.042; * 10 * 4-9 1-5 0.042; * 11 * 4-9 1-5 0.053;"
		"* 12 * 4-9 1-5 0.057; * 13 * 4-9 1-5 0.046; * 14 * 4-9 1-5 0.044; * 15 * 4-9 1-5 0.053;"
		"* 16 * 4-9 1-5 0.094; * 17 * 4-9 1-5 0.168; * 18 * 4-9 1-5 0.148; * 19 * 4-9 1-5 0.086;"
		"* 20 * 4-9 1-5 0.053; * 21 * 4-9 1-5 0.038; * 22 * 4-9 1-5 0.023; * 23 * 4-9 1-5 0.013"
		"}"
		"weekend-summer {"
		"*  0 * 4-9 6-0 0.009; *  1 * 4-9 6-0 0.007; *  2 * 4-9 6-0 0.007; *  3 * 4-9 6-0 0.007;"
		"*  4 * 4-9 6-0 0.007; *  5 * 4-9 6-0 0.009; *  6 * 4-9 6-0 0.017; *  7 * 4-9 6-0 0.038;"
		"*  8 * 4-9 6-0 0.060; *  9 * 4-9 6-0 0.068; * 10 * 4-9 6-0 0.065; * 11 * 4-9 6-0 0.067;"
		"* 12 * 4-9 6-0 0.076; * 13 * 4-9 6-0 0.066; * 14 * 4-9 6-0 0.061; * 15 * 4-9 6-0 0.067;"
		"* 16 * 4-9 6-0 0.091; * 17 * 4-9 6-0 0.134; * 18 * 4-9 6-0 0.121; * 19 * 4-9 6-0 0.080;"
		"* 20 * 4-9 6-0 0.052; * 21 * 4-9 6-0 0.035; * 22 * 4-9 6-0 0.022; * 23 * 4-9 6-0 0.011"
		"}"
		"weekday-winter {"
		"*  0 * 10-3 1-5 0.010; *  1 * 10-3 1-5 0.009; *  2 * 10-3 1-5 0.009; *  3 * 10-3 1-5 0.009;"
		"*  4 * 10-3 1-5 0.009; *  5 * 10-3 1-5 0.016; *  6 * 10-3 1-5 0.032; *  7 * 10-3 1-5 0.050;"
		"*  8 * 10-3 1-5 0.045; *  9 * 10-3 1-5 0.043; * 10 * 10-3 1-5 0.045; * 11 * 10-3 1-5 0.059;"
		"* 12 * 10-3 1-5 0.063; * 13 * 10-3 1-5 0.053; * 14 * 10-3 1-5 0.052; * 15 * 10-3 1-5 0.072;"
		"* 16 * 10-3 1-5 0.138; * 17 * 10-3 1-5 0.242; * 18 * 10-3 1-5 0.182; * 19 * 10-3 1-5 0.088;"
		"* 20 * 10-3 1-5 0.051; * 21 * 10-3 1-5 0.034; * 22 * 10-3 1-5 0.022; * 23 * 10-3 1-5 0.014"
		"}"
		"weekend-winter {"
		"*  0 * 10-3 6-0 0.013; *  1 * 10-3 6-0 0.010; *  2 * 10-3 6-0 0.010; *  3 * 10-3 6-0 0.010;"
		"*  4 * 10-3 6-0 0.010; *  5 * 10-3 6-0 0.012; *  6 * 10-3 6-0 0.018; *  7 * 10-3 6-0 0.040;"
		"*  8 * 10-3 6-0 0.073; *  9 * 10-3 6-0 0.094; * 10 * 10-3 6-0 0.091; * 11 * 10-3 6-0 0.100;"
		"* 12 * 10-3 6-0 0.117; * 13 * 10-3 6-0 0.109; * 14 * 10-3 6-0 0.100; * 15 * 10-3 6-0 0.108;"
		"* 16 * 10-3 6-0 0.153;	* 17 * 10-3 6-0 0.215; * 18 * 10-3 6-0 0.161; * 19 * 10-3 6-0 0.085;"
		"* 20 * 10-3 6-0 0.050;	* 21 * 10-3 6-0 0.033; * 22 * 10-3 6-0 0.022; * 23 * 10-3 6-0 0.014"
		"}"
	},
	{	"SUMP",
		{20,false,{0,0,1},0.9,0.0},
		"type:analog; schedule: residential-sump-default; power: 0.8 kW",
		"residential-sump-default",
		"positive; nonzero; interpolated; * * * * * 1.0;"
	},
	/// @todo add other implicit enduse schedules and shapes as they are defined
	{NULL}
};
