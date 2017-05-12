#ifndef RT_PPG102A1_H
#define RT_PPG102A1_H

// For USSensor PPG102A1 (Platinum RTD)
static const double thermistor_curve[][2] = {
	{ -50, 803.14 },
	{ -49, 807.11 },
	{ -48, 811.07 },
	{ -47, 815.03 },
	{ -46, 819.00 },
	{ -45, 822.96 },
	{ -44, 826.92 },
	{ -43, 830.88 },
	{ -42, 834.83 },
	{ -41, 838.79 },
	{ -40, 842.74 },
	{ -39, 846.70 },
	{ -38, 850.65 },
	{ -37, 854.60 },
	{ -36, 858.55 },
	{ -35, 862.50 },
	{ -34, 866.45 },
	{ -33, 870.40 },
	{ -32, 874.34 },
	{ -31, 878.29 },
	{ -30, 882.23 },
	{ -29, 886.17 },
	{ -28, 890.11 },
	{ -27, 894.05 },
	{ -26, 897.99 },
	{ -25, 901.93 },
	{ -24, 905.87 },
	{ -23, 909.80 },
	{ -22, 913.74 },
	{ -21, 917.67 },
	{ -20, 921.60 },
	{ -19, 925.53 },
	{ -18, 929.46 },
	{ -17, 933.39 },
	{ -16, 937.32 },
	{ -15, 941.25 },
	{ -14, 945.17 },
	{ -13, 949.09 },
	{ -12, 953.02 },
	{ -11, 956.94 },
	{ -10, 960.86 },
	{ -9, 964.78 },
	{ -8, 968.70 },
	{ -7, 972.61 },
	{ -6, 976.53 },
	{ -5, 980.44 },
	{ -4, 984.36 },
	{ -3, 988.27 },
	{ -2, 992.18 },
	{ -1, 996.09 },
	{ 0, 1000.00 },
	{ 1, 1003.91 },
	{ 2, 1007.81 },
	{ 3, 1011.72 },
	{ 4, 1015.62 },
	{ 5, 1019.53 },
	{ 6, 1023.43 },
	{ 7, 1027.33 },
	{ 8, 1031.23 },
	{ 9, 1035.13 },
	{ 10, 1039.03 },
	{ 11, 1042.92 },
	{ 12, 1046.82 },
	{ 13, 1050.71 },
	{ 14, 1054.60 },
	{ 15, 1058.49 },
	{ 16, 1062.38 },
	{ 17, 1066.27 },
	{ 18, 1070.16 },
	{ 19, 1074.05 },
	{ 20, 1077.94 },
	{ 21, 1081.82 },
	{ 22, 1085.70 },
	{ 23, 1089.59 },
	{ 24, 1093.47 },
	{ 25, 1097.35 },
	{ 26, 1101.23 },
	{ 27, 1105.10 },
	{ 28, 1108.98 },
	{ 29, 1112.86 },
	{ 30, 1116.73 },
	{ 31, 1120.60 },
	{ 32, 1124.47 },
	{ 33, 1128.35 },
	{ 34, 1132.21 },
	{ 35, 1136.08 },
	{ 36, 1139.95 },
	{ 37, 1143.82 },
	{ 38, 1147.68 },
	{ 39, 1151.55 },
	{ 40, 1155.41 },
	{ 41, 1159.27 },
	{ 42, 1163.13 },
	{ 43, 1166.99 },
	{ 44, 1170.85 },
	{ 45, 1174.70 },
	{ 46, 1178.56 },
	{ 47, 1182.41 },
	{ 48, 1186.27 },
	{ 49, 1190.12 },
	{ 50, 1193.97 },
	{ 51, 1197.82 },
	{ 52, 1201.67 },
	{ 53, 1205.52 },
	{ 54, 1209.36 },
	{ 55, 1213.21 },
	{ 56, 1217.05 },
	{ 57, 1220.90 },
	{ 58, 1224.74 },
	{ 59, 1228.58 },
	{ 60, 1232.42 },
	{ 61, 1236.26 },
	{ 62, 1240.09 },
	{ 63, 1243.93 },
	{ 64, 1247.77 },
	{ 65, 1251.60 },
	{ 66, 1255.43 },
	{ 67, 1259.26 },
	{ 68, 1263.09 },
	{ 69, 1266.92 },
	{ 70, 1270.75 },
	{ 71, 1274.58 },
	{ 72, 1278.40 },
	{ 73, 1282.23 },
	{ 74, 1286.05 },
	{ 75, 1289.87 },
	{ 76, 1293.70 },
	{ 77, 1297.52 },
	{ 78, 1301.33 },
	{ 79, 1305.15 },
	{ 80, 1308.97 },
	{ 81, 1312.78 },
	{ 82, 1316.60 },
	{ 83, 1320.41 },
	{ 84, 1324.22 },
	{ 85, 1328.03 },
	{ 86, 1331.84 },
	{ 87, 1335.65 },
	{ 88, 1339.46 },
	{ 89, 1343.26 },
	{ 90, 1347.07 },
	{ 91, 1350.87 },
	{ 92, 1354.68 },
	{ 93, 1358.48 },
	{ 94, 1362.28 },
	{ 95, 1366.08 },
	{ 96, 1369.87 },
	{ 97, 1373.67 },
	{ 98, 1377.47 },
	{ 99, 1381.26 },
	{ 100, 1385.06 },
	{ 101, 1388.85 },
	{ 102, 1392.64 },
	{ 103, 1396.43 },
	{ 104, 1400.22 },
	{ 105, 1404.00 },
	{ 106, 1407.79 },
	{ 107, 1411.58 },
	{ 108, 1415.36 },
	{ 109, 1419.14 },
	{ 110, 1422.93 },
	{ 111, 1426.71 },
	{ 112, 1430.49 },
	{ 113, 1434.26 },
	{ 114, 1438.04 },
	{ 115, 1441.82 },
	{ 116, 1445.59 },
	{ 117, 1449.37 },
	{ 118, 1453.14 },
	{ 119, 1456.91 },
	{ 120, 1460.68 },
	{ 121, 1464.45 },
	{ 122, 1468.22 },
	{ 123, 1471.98 },
	{ 124, 1475.75 },
	{ 125, 1479.51 },
	{ 126, 1483.28 },
	{ 127, 1487.04 },
	{ 128, 1490.80 },
	{ 129, 1494.56 },
	{ 130, 1498.32 },
	{ 131, 1502.08 },
	{ 132, 1505.83 },
	{ 133, 1509.59 },
	{ 134, 1513.34 },
	{ 135, 1517.10 },
	{ 136, 1520.85 },
	{ 137, 1524.60 },
	{ 138, 1528.35 },
	{ 139, 1532.10 },
	{ 140, 1535.84 },
	{ 141, 1539.59 },
	{ 142, 1543.33 },
	{ 143, 1547.08 },
	{ 144, 1550.82 },
	{ 145, 1554.56 },
	{ 146, 1558.30 },
	{ 147, 1562.04 },
	{ 148, 1565.78 },
	{ 149, 1569.52 },
	{ 150, 1573.25 },
	{ 151, 1576.99 },
	{ 152, 1580.72 },
	{ 153, 1584.45 },
	{ 154, 1588.18 },
	{ 155, 1591.91 },
	{ 156, 1595.64 },
	{ 157, 1599.37 },
	{ 158, 1603.09 },
	{ 159, 1606.82 },
	{ 160, 1610.54 },
	{ 161, 1614.27 },
	{ 162, 1617.99 },
	{ 163, 1621.71 },
	{ 164, 1625.43 },
	{ 165, 1629.15 },
	{ 166, 1632.86 },
	{ 167, 1636.58 },
	{ 168, 1640.30 },
	{ 169, 1644.01 },
	{ 170, 1647.72 },
	{ 171, 1651.43 },
	{ 172, 1655.14 },
	{ 173, 1658.85 },
	{ 174, 1662.56 },
	{ 175, 1666.27 },
	{ 176, 1669.97 },
	{ 177, 1673.68 },
	{ 178, 1677.38 },
	{ 179, 1681.08 },
	{ 180, 1684.78 },
	{ 181, 1688.48 },
	{ 182, 1692.18 },
	{ 183, 1695.88 },
	{ 184, 1699.58 },
	{ 185, 1703.27 },
	{ 186, 1706.96 },
	{ 187, 1710.66 },
	{ 188, 1714.35 },
	{ 189, 1718.04 },
	{ 190, 1721.73 },
	{ 191, 1725.42 },
	{ 192, 1729.10 },
	{ 193, 1732.79 },
	{ 194, 1736.48 },
	{ 195, 1740.16 },
	{ 196, 1743.84 },
	{ 197, 1747.52 },
	{ 198, 1751.20 },
	{ 199, 1754.88 },
	{ 200, 1758.56 },
	{ 201, 1762.24 },
	{ 202, 1765.91 },
	{ 203, 1769.59 },
	{ 204, 1773.26 },
	{ 205, 1776.93 },
	{ 206, 1780.60 },
	{ 207, 1784.27 },
	{ 208, 1787.94 },
	{ 209, 1791.61 },
	{ 210, 1795.28 },
	{ 211, 1798.94 },
	{ 212, 1802.60 },
	{ 213, 1806.27 },
	{ 214, 1809.93 },
	{ 215, 1813.59 },
	{ 216, 1817.25 },
	{ 217, 1820.91 },
	{ 218, 1824.56 },
	{ 219, 1828.22 },
	{ 220, 1831.88 },
	{ 221, 1835.53 },
	{ 222, 1839.18 },
	{ 223, 1842.83 },
	{ 224, 1846.48 },
	{ 225, 1850.13 },
	{ 226, 1853.78 },
	{ 227, 1857.43 },
	{ 228, 1861.07 },
	{ 229, 1864.72 },
	{ 230, 1868.36 },
	{ 231, 1872.00 },
	{ 232, 1875.64 },
	{ 233, 1879.28 },
	{ 234, 1882.92 },
	{ 235, 1886.56 },
	{ 236, 1890.19 },
	{ 237, 1893.83 },
	{ 238, 1897.46 },
	{ 239, 1901.10 },
	{ 240, 1904.73 },
	{ 241, 1908.36 },
	{ 242, 1911.99 },
	{ 243, 1915.62 },
	{ 244, 1919.24 },
	{ 245, 1922.87 },
	{ 246, 1926.49 },
	{ 247, 1930.12 },
	{ 248, 1933.74 },
	{ 249, 1937.36 },
	{ 250, 1940.98 },
	{ 251, 1944.60 },
	{ 252, 1948.22 },
	{ 253, 1951.83 },
	{ 254, 1955.45 },
	{ 255, 1959.06 },
	{ 256, 1962.68 },
	{ 257, 1966.29 },
	{ 258, 1969.90 },
	{ 259, 1973.51 },
	{ 260, 1977.12 },
	{ 261, 1980.73 },
	{ 262, 1984.33 },
	{ 263, 1987.94 },
	{ 264, 1991.54 },
	{ 265, 1995.14 },
	{ 266, 1998.75 },
	{ 267, 2002.35 },
	{ 268, 2005.95 },
	{ 269, 2009.54 },
	{ 270, 2013.14 },
	{ 271, 2016.74 },
	{ 272, 2020.33 },
	{ 273, 2023.93 },
	{ 274, 2027.52 },
	{ 275, 2031.11 },
	{ 276, 2034.70 },
	{ 277, 2038.29 },
	{ 278, 2041.88 },
	{ 279, 2045.46 },
	{ 280, 2049.05 },
	{ 281, 2052.63 },
	{ 282, 2056.22 },
	{ 283, 2059.80 },
	{ 284, 2063.38 },
	{ 285, 2066.96 },
	{ 286, 2070.54 },
	{ 287, 2074.11 },
	{ 288, 2077.69 },
	{ 289, 2081.27 },
	{ 290, 2084.84 },
	{ 291, 2088.41 },
	{ 292, 2091.98 },
	{ 293, 2095.55 },
	{ 294, 2099.12 },
	{ 295, 2102.69 },
	{ 296, 2106.26 },
	{ 297, 2109.82 },
	{ 298, 2113.39 },
	{ 299, 2116.95 },
	{ 300, 2120.52 },
	{ 301, 2124.08 },
	{ 302, 2127.64 },
	{ 303, 2131.20 },
	{ 304, 2134.75 },
	{ 305, 2138.31 },
	{ 306, 2141.87 },
	{ 307, 2145.42 },
	{ 308, 2148.97 },
	{ 309, 2152.52 },
	{ 310, 2156.08 },
	{ 311, 2159.62 },
	{ 312, 2163.17 },
	{ 313, 2166.72 },
	{ 314, 2170.27 },
	{ 315, 2173.81 },
	{ 316, 2177.36 },
	{ 317, 2180.90 },
	{ 318, 2184.44 },
	{ 319, 2187.98 },
	{ 320, 2191.52 },
	{ 321, 2195.06 },
	{ 322, 2198.60 },
	{ 323, 2202.13 },
	{ 324, 2205.67 },
	{ 325, 2209.20 },
	{ 326, 2212.73 },
	{ 327, 2216.26 },
	{ 328, 2219.79 },
	{ 329, 2223.32 },
	{ 330, 2226.85 },
	{ 331, 2230.38 },
	{ 332, 2233.90 },
	{ 333, 2237.43 },
	{ 334, 2240.95 },
	{ 335, 2244.47 },
	{ 336, 2247.99 },
	{ 337, 2251.51 },
	{ 338, 2255.03 },
	{ 339, 2258.55 },
	{ 340, 2262.06 },
	{ 341, 2265.58 },
	{ 342, 2269.09 },
	{ 343, 2272.60 },
	{ 344, 2276.12 },
	{ 345, 2279.63 },
	{ 346, 2283.14 },
	{ 347, 2286.64 },
	{ 348, 2290.15 },
	{ 349, 2293.66 },
	{ 350, 2297.16 },
	{ 351, 2300.66 },
	{ 352, 2304.17 },
	{ 353, 2307.67 },
	{ 354, 2311.17 },
	{ 355, 2314.67 },
	{ 356, 2318.16 },
	{ 357, 2321.66 },
	{ 358, 2325.16 },
	{ 359, 2328.65 },
	{ 360, 2332.14 },
	{ 361, 2335.64 },
	{ 362, 2339.13 },
	{ 363, 2342.62 },
	{ 364, 2346.10 },
	{ 365, 2349.59 },
	{ 366, 2353.08 },
	{ 367, 2356.56 },
	{ 368, 2360.05 },
	{ 369, 2363.53 },
	{ 370, 2367.01 },
	{ 371, 2370.49 },
	{ 372, 2373.97 },
	{ 373, 2377.45 },
	{ 374, 2380.93 },
	{ 375, 2384.40 },
	{ 376, 2387.88 },
	{ 377, 2391.35 },
	{ 378, 2394.82 },
	{ 379, 2398.29 },
	{ 380, 2401.76 },
	{ 381, 2405.23 },
	{ 382, 2408.70 },
	{ 383, 2412.17 },
	{ 384, 2415.63 },
	{ 385, 2419.10 },
	{ 386, 2422.56 },
	{ 387, 2426.02 },
	{ 388, 2429.48 },
	{ 389, 2432.94 },
	{ 390, 2436.40 },
	{ 391, 2439.86 },
	{ 392, 2443.31 },
	{ 393, 2446.77 },
	{ 394, 2450.22 },
	{ 395, 2453.67 },
	{ 396, 2457.13 },
	{ 397, 2460.58 },
	{ 398, 2464.03 },
	{ 399, 2467.47 },
	{ 400, 2470.92 },
	{ 401, 2474.37 },
	{ 402, 2477.81 },
	{ 403, 2481.25 },
	{ 404, 2484.70 },
	{ 405, 2488.14 },
	{ 406, 2491.58 },
	{ 407, 2495.02 },
	{ 408, 2498.45 },
	{ 409, 2501.89 },
	{ 410, 2505.33 },
	{ 411, 2508.76 },
	{ 412, 2512.19 },
	{ 413, 2515.62 },
	{ 414, 2519.06 },
	{ 415, 2522.48 },
	{ 416, 2525.91 },
	{ 417, 2529.34 },
	{ 418, 2532.77 },
	{ 419, 2536.19 },
	{ 420, 2539.62 },
	{ 421, 2543.04 },
	{ 422, 2546.46 },
	{ 423, 2549.88 },
	{ 424, 2553.30 },
	{ 425, 2556.72 },
	{ 426, 2560.13 },
	{ 427, 2563.55 },
	{ 428, 2566.96 },
	{ 429, 2570.38 },
	{ 430, 2573.79 },
	{ 431, 2577.20 },
	{ 432, 2580.61 },
	{ 433, 2584.02 },
	{ 434, 2587.43 },
	{ 435, 2590.83 },
	{ 436, 2594.24 },
	{ 437, 2597.64 },
	{ 438, 2601.05 },
	{ 439, 2604.45 },
	{ 440, 2607.85 },
	{ 441, 2611.25 },
	{ 442, 2614.65 },
	{ 443, 2618.04 },
	{ 444, 2621.44 },
	{ 445, 2624.83 },
	{ 446, 2628.23 },
	{ 447, 2631.62 },
	{ 448, 2635.01 },
	{ 449, 2638.40 },
	{ 450, 2641.79 },
	{ 451, 2645.18 },
	{ 452, 2648.57 },
	{ 453, 2651.95 },
	{ 454, 2655.34 },
	{ 455, 2658.72 },
	{ 456, 2662.10 },
	{ 457, 2665.48 },
	{ 458, 2668.86 },
	{ 459, 2672.24 },
	{ 460, 2675.62 },
	{ 461, 2679.00 },
	{ 462, 2682.37 },
	{ 463, 2685.74 },
	{ 464, 2689.12 },
	{ 465, 2692.49 },
	{ 466, 2695.86 },
	{ 467, 2699.23 },
	{ 468, 2702.60 },
	{ 469, 2705.97 },
	{ 470, 2709.33 },
	{ 471, 2712.70 },
	{ 472, 2716.06 },
	{ 473, 2719.42 },
	{ 474, 2722.78 },
	{ 475, 2726.14 },
	{ 476, 2729.50 },
	{ 477, 2732.86 },
	{ 478, 2736.22 },
	{ 479, 2739.57 },
	{ 480, 2742.93 },
	{ 481, 2746.28 },
	{ 482, 2749.63 },
	{ 483, 2752.98 },
	{ 484, 2756.33 },
	{ 485, 2759.68 },
	{ 486, 2763.03 },
	{ 487, 2766.38 },
	{ 488, 2769.72 },
	{ 489, 2773.07 },
	{ 490, 2776.41 },
	{ 491, 2779.75 },
	{ 492, 2783.09 },
	{ 493, 2786.43 },
	{ 494, 2789.77 },
	{ 495, 2793.11 },
	{ 496, 2796.44 },
	{ 497, 2799.78 },
	{ 498, 2803.11 },
	{ 499, 2806.44 },
    { 500, 2809.78 }
};

#endif // RT_PPG102A1_H