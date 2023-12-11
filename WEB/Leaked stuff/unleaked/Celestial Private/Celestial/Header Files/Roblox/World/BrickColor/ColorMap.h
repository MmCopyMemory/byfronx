#pragma once

#include <Windows.h>
#include <vector>
#include <string>

/*Include this to use the ImColor*/
#include <ImGui/imgui.h>

class CustomColorMap
{
public:

	enum BrickColor {
		brick_1 = 1,
		brick_2 = 2,
		brick_3 = 3,
		brick_5 = 5,
		brick_6 = 6,
		brick_9 = 9,
		brick_11 = 11,
		brick_12 = 12,
		brick_18 = 18,
		brick_21 = 21,
		brick_22 = 22,
		brick_23 = 23,
		brick_24 = 24,
		brick_25 = 25,
		brick_26 = 26,
		brick_27 = 27,
		brick_28 = 28,
		brick_29 = 29,
		brick_36 = 36,
		brick_37 = 37,
		brick_38 = 38,
		brick_39 = 39,
		brick_40 = 40,
		brick_41 = 41,
		brick_42 = 42,
		brick_43 = 43,
		brick_44 = 44,
		brick_45 = 45,
		brick_47 = 47,
		brick_48 = 48,
		brick_49 = 49,
		brick_50 = 50,
		brick_100 = 100,
		brick_101 = 101,
		brick_102 = 102,
		brick_103 = 103,
		brick_104 = 104,
		brick_105 = 105,
		brick_106 = 106,
		brick_107 = 107,
		brick_108 = 108,
		brick_110 = 110,
		brick_111 = 111,
		brick_112 = 112,
		brick_113 = 113,
		brick_115 = 115,
		brick_116 = 116,
		brick_118 = 118,
		brick_119 = 119,
		brick_120 = 120,
		brick_121 = 121,
		brick_123 = 123,
		brick_124 = 124,
		brick_125 = 125,
		brick_126 = 126,
		brick_127 = 127,
		brick_128 = 128,
		brick_131 = 131,
		brick_133 = 133,
		brick_134 = 134,
		brick_135 = 135,
		brick_136 = 136,
		brick_137 = 137,
		brick_138 = 138,
		brick_140 = 140,
		brick_141 = 141,
		brick_143 = 143,
		brick_145 = 145,
		brick_146 = 146,
		brick_147 = 147,
		brick_148 = 148,
		brick_149 = 149,
		brick_150 = 150,
		brick_151 = 151,
		brick_153 = 153,
		brick_154 = 154,
		brick_157 = 157,
		brick_158 = 158,
		brick_168 = 168,
		brick_176 = 176,
		brick_178 = 178,
		brick_179 = 179,
		brick_180 = 180,
		brick_190 = 190,
		brick_191 = 191,
		brick_192 = 192,
		brick_193 = 193,
		brick_194 = 194,
		brick_195 = 195,
		brick_196 = 196,
		brick_198 = 198,
		brick_199 = 199,
		brick_200 = 200,
		brick_208 = 208,
		brick_209 = 209,
		brick_210 = 210,
		brick_211 = 211,
		brick_212 = 212,
		brick_213 = 213,
		brick_216 = 216,
		brick_217 = 217,
		brick_218 = 218,
		brick_219 = 219,
		brick_220 = 220,
		brick_221 = 221,
		brick_222 = 222,
		brick_223 = 223,
		brick_224 = 224,
		brick_225 = 225,
		brick_226 = 226,
		brick_232 = 232,
		brick_268 = 268,
		brick_301 = 301,
		brick_302 = 302,
		brick_303 = 303,
		brick_304 = 304,
		brick_305 = 305,
		brick_306 = 306,
		brick_307 = 307,
		brick_308 = 308,
		brick_309 = 309,
		brick_310 = 310,
		brick_311 = 311,
		brick_312 = 312,
		brick_313 = 313,
		brick_314 = 314,
		brick_315 = 315,
		brick_316 = 316,
		brick_317 = 317,
		brick_318 = 318,
		brick_319 = 319,
		brick_320 = 320,
		brick_321 = 321,
		brick_322 = 322,
		brick_323 = 323,
		brick_324 = 324,
		brick_325 = 325,
		//brick_326 = 326,
		brick_327 = 327,
		brick_328 = 328,
		brick_329 = 329,
		brick_330 = 330,
		brick_331 = 331,
		brick_332 = 332,
		brick_333 = 333,
		brick_334 = 334,
		brick_335 = 335,
		brick_336 = 336,
		brick_337 = 337,
		brick_338 = 338,
		brick_339 = 339,
		brick_340 = 340,
		brick_341 = 341,
		brick_342 = 342,
		brick_343 = 343,
		brick_344 = 344,
		brick_345 = 345,
		brick_346 = 346,
		brick_347 = 347,
		brick_348 = 348,
		brick_349 = 349,
		brick_350 = 350,
		brick_351 = 351,
		brick_352 = 352,
		brick_353 = 353,
		brick_354 = 354,
		brick_355 = 355,
		brick_356 = 356,
		brick_357 = 357,
		brick_358 = 358,
		brick_359 = 359,
		brick_360 = 360,
		brick_361 = 361,
		brick_362 = 362,
		brick_363 = 363,
		brick_364 = 364,
		brick_365 = 365,
		roblox_1001 = 1001,
		roblox_1002 = 1002,
		roblox_1003 = 1003,
		roblox_1004 = 1004,
		roblox_1005 = 1005,
		roblox_1006 = 1006,
		roblox_1007 = 1007,
		roblox_1008 = 1008,
		roblox_1009 = 1009,
		roblox_1010 = 1010,
		roblox_1011 = 1011,
		roblox_1012 = 1012,
		roblox_1013 = 1013,
		roblox_1014 = 1014,
		roblox_1015 = 1015,
		roblox_1016 = 1016,
		roblox_1017 = 1017,
		roblox_1018 = 1018,
		roblox_1019 = 1019,
		roblox_1020 = 1020,
		roblox_1021 = 1021,
		roblox_1022 = 1022,
		roblox_1023 = 1023,
		roblox_1024 = 1024,
		roblox_1025 = 1025,
		roblox_1026 = 1026,
		roblox_1027 = 1027,
		roblox_1028 = 1028,
		roblox_1029 = 1029,
		roblox_1030 = 1030,
		roblox_1031 = 1031,
		roblox_1032 = 1032
	};

	ImU32 GetColorFromID(std::uintptr_t ID);
	std::string ColorName;
private:
};

ImU32 CustomColorMap::GetColorFromID(std::uintptr_t ID)
{
	switch (ID)
	{
	case brick_1:
	{
		this->ColorName = "White";
		return ImColor(242, 243, 243);
		break;
	}

	case brick_2:
	{
		this->ColorName = "Gray";
		return ImColor(161, 165, 162);
		break;
	}

	case brick_3:
	{
		this->ColorName = "Light yellow";
		return ImColor(249, 233, 153);
		break;
	}

	case brick_5:
	{
		this->ColorName = "Brick yellow";
		return ImColor(215, 197, 154);
		break;
	}

	case brick_6:
	{
		this->ColorName = "Light green (Mint)";
		return ImColor(194, 218, 184);
		break;
	}

	case brick_9:
	{
		this->ColorName = "Light reddish violet";
		return ImColor(232, 186, 200);
		break;
	}

	case brick_11:
	{
		this->ColorName = "Pastel Blue";
		return ImColor(0x80, 0xbb, 0xdb);
		break;
	}

	case brick_12:
	{
		this->ColorName = "Light orange brown";
		return ImColor(203, 132, 66);
		break;
	}

	case brick_18:
	{
		this->ColorName = "Nougat";
		return ImColor(204, 142, 105);
		break;
	}

	case brick_21:
	{
		this->ColorName = "Bright red";
		return ImColor(196, 40, 28);
		break;
	}

	case brick_22:
	{
		this->ColorName = "Med. reddish violet";
		return ImColor(196, 112, 160);
		break;
	}

	case brick_23:
	{
		this->ColorName = "Bright blue";
		return ImColor(13, 105, 172);
		break;
	}

	case brick_24:
	{
		this->ColorName = "Bright yellow";
		return ImColor(245, 205, 48);
		break;
	}

	case brick_25:
	{
		this->ColorName = "Earth orange";
		return ImColor(98, 71, 50);
		break;
	}

	case brick_26:
	{
		this->ColorName = "Black";
		return ImColor(27, 42, 53);
		break;
	}

	case brick_27:
	{
		this->ColorName = "Dark grey";
		return ImColor(109, 110, 108);
		break;
	}

	case brick_28:
	{
		this->ColorName = "Dark green";
		return ImColor(40, 127, 71);
		break;
	}

	case brick_29:
	{
		this->ColorName = "Medium green";
		return ImColor(161, 196, 140);
		break;
	}

	case brick_36:
	{
		this->ColorName = "Lig. Yellowich orange";
		return ImColor(243, 207, 155);
		break;
	}

	case brick_37:
	{
		this->ColorName = "Bright green";
		return ImColor(75, 151, 75);
		break;
	}

	case brick_38:
	{
		this->ColorName = "Dark orange";
		return ImColor(160, 95, 53);
		break;
	}

	case brick_39:
	{
		this->ColorName = "Light bluish violet";
		return ImColor(193, 202, 222);
		break;
	}

	case brick_40:
	{
		this->ColorName = "Transparent";
		return ImColor(236, 236, 236);
		break;
	}

	case brick_41:
	{
		this->ColorName = "Tr. Red";
		return ImColor(205, 84, 75);
		break;
	}

	case brick_42:
	{
		this->ColorName = "Tr. Lg blue";
		return ImColor(193, 223, 240);
		break;
	}

	case brick_43:
	{
		this->ColorName = "Tr. Blue";
		return ImColor(123, 182, 232);
		break;
	}

	case brick_44:
	{
		this->ColorName = "Tr. Yellow";
		return ImColor(247, 241, 141);
		break;
	}

	case brick_45:
	{
		this->ColorName = "Light blue";
		return ImColor(180, 210, 228);
		break;
	}

	case brick_47:
	{
		this->ColorName = "Tr. Flu. Reddish orange";
		return ImColor(217, 133, 108);
		break;
	}

	case brick_48:
	{
		this->ColorName = "Tr. Green";
		return ImColor(132, 182, 141);
		break;
	}

	case brick_49:
	{
		this->ColorName = "Tr. Flu. Green";
		return ImColor(248, 241, 132);
		break;
	}

	case brick_50:
	{
		this->ColorName = "Phosph. White";
		return ImColor(236, 232, 222);
		break;
	}

	case brick_100:
	{
		this->ColorName = "Light red";
		return ImColor(238, 196, 182);
		break;
	}

	case brick_101:
	{
		this->ColorName = "Medium red";
		return ImColor(218, 134, 122);
		break;
	}

	case brick_102:
	{
		this->ColorName = "Medium blue";
		return ImColor(110, 153, 202);
		break;
	}

	case brick_103:
	{
		this->ColorName = "Light grey";
		return ImColor(199, 193, 183);
		break;
	}

	case brick_104:
	{
		this->ColorName = "Bright violet";
		return ImColor(107, 50, 124);
		break;
	}

	case brick_105:
	{
		this->ColorName = "Br. yellowish orange";
		return ImColor(226, 155, 64);
		break;
	}

	case brick_106:
	{
		this->ColorName = "Bright orange";
		return ImColor(218, 133, 65);
		break;
	}

	case brick_107:
	{
		this->ColorName = "Bright bluish green";
		return ImColor(0, 143, 156);
		break;
	}

	case brick_108:
	{
		this->ColorName = "Earth yellow";
		return ImColor(104, 92, 67);
		break;
	}

	case brick_110:
	{
		this->ColorName = "Bright bluish violet";
		return ImColor(67, 84, 147);
		break;
	}

	case brick_111:
	{
		this->ColorName = "Tr. Brown";
		return ImColor(191, 183, 177);
		break;
	}

	case brick_112:
	{
		this->ColorName = "Medium bluish violet";
		return ImColor(104, 116, 172);
		break;
	}

	case brick_113:
	{
		this->ColorName = "Tr. Medi. reddish violet";
		return ImColor(228, 173, 200);
		break;
	}

	case brick_115:
	{
		this->ColorName = "Med. yellowish green";
		return ImColor(199, 210, 60);
		break;
	}

	case brick_116:
	{
		this->ColorName = "Med. bluish green";
		return ImColor(85, 165, 175);
		break;
	}

	case brick_118:
	{
		this->ColorName = "Light bluish green";
		return ImColor(183, 215, 213);
		break;
	}

	case brick_119:
	{
		this->ColorName = "Br. yellowish green";
		return ImColor(164, 189, 71);
		break;
	}

	case brick_120:
	{
		this->ColorName = "Lig. yellowish green";
		return ImColor(217, 228, 167);
		break;
	}

	case brick_121:
	{
		this->ColorName = "Med. yellowish orange";
		return ImColor(231, 172, 88);
		break;
	}

	case brick_123:
	{
		this->ColorName = "Br. reddish orange";
		return ImColor(211, 111, 76);
		break;
	}

	case brick_124:
	{
		this->ColorName = "Bright reddish violet";
		return ImColor(146, 57, 120);
		break;
	}

	case brick_125:
	{
		this->ColorName = "Light orange";
		return ImColor(234, 184, 146);
		break;
	}

	case brick_126:
	{
		this->ColorName = "Tr. Bright bluish violet";
		return ImColor(165, 165, 203);
		break;
	}

	case brick_127:
	{
		this->ColorName = "Gold";
		return ImColor(220, 188, 129);
		break;
	}

	case brick_128:
	{
		this->ColorName = "Dark nougat";
		return ImColor(174, 122, 89);
		break;
	}

	case brick_131:
	{
		this->ColorName = "Silver";
		return ImColor(156, 163, 168);
		break;
	}

	case brick_133:
	{
		this->ColorName = "Neon orange";
		return ImColor(213, 115, 61);
		break;
	}

	case brick_134:
	{
		this->ColorName = "Neon green";
		return ImColor(216, 221, 86);
		break;
	}

	case brick_135:
	{
		this->ColorName = "Sand blue";
		return ImColor(116, 134, 157);
		break;
	}

	case brick_136:
	{
		this->ColorName = "Sand violet";
		return ImColor(135, 124, 144);
		break;
	}

	case brick_137:
	{
		this->ColorName = "Medium orange";
		return ImColor(224, 152, 100);
		break;
	}

	case brick_138:
	{
		this->ColorName = "Sand yellow";
		return ImColor(149, 138, 115);
		break;
	}

	case brick_140:
	{
		this->ColorName = "Earth blue";
		return ImColor(32, 58, 86);
		break;
	}

	case brick_141:
	{
		this->ColorName = "Earth green";
		return ImColor(39, 70, 45);
		break;
	}

	case brick_143:
	{
		this->ColorName = "Tr. Flu. Blue";
		return ImColor(207, 226, 247);
		break;
	}

	case brick_145:
	{
		this->ColorName = "Sand blue metallic";
		return ImColor(121, 136, 161);
		break;
	}

	case brick_146:
	{
		this->ColorName = "Sand violet metallic";
		return ImColor(149, 142, 163);
		break;
	}

	case brick_147:
	{
		this->ColorName = "Sand yellow metallic";
		return ImColor(147, 135, 103);
		break;
	}

	case brick_148:
	{
		this->ColorName = "Dark grey metallic";
		return ImColor(87, 88, 87);
		break;
	}

	case brick_149:
	{
		this->ColorName = "Black metallic";
		return ImColor(22, 29, 50);
		break;
	}

	case brick_150:
	{
		this->ColorName = "Light grey metallic";
		return ImColor(171, 173, 172);
		break;
	}

	case brick_151:
	{
		this->ColorName = "Sand green";
		return ImColor(120, 144, 130);
		break;
	}

	case brick_153:
	{
		this->ColorName = "Sand red";
		return ImColor(149, 121, 119);
		break;
	}

	case brick_154:
	{
		this->ColorName = "Dark red";
		return ImColor(123, 46, 47);
		break;
	}

	case brick_157:
	{
		this->ColorName = "Tr. Flu. Yellow";
		return ImColor(255, 246, 123);
		break;
	}

	case brick_158:
	{
		this->ColorName = "Tr. Flu. Red";
		return ImColor(225, 164, 194);
		break;
	}

	case brick_168:
	{
		this->ColorName = "Gun metallic";
		return ImColor(117, 108, 98);
		break;
	}

	case brick_176:
	{
		this->ColorName = "Red flip/flop";
		return ImColor(151, 105, 91);
		break;
	}

	case brick_178:
	{
		this->ColorName = "Yellow flip/flop";
		return ImColor(180, 132, 85);
		break;
	}

	case brick_179:
	{
		this->ColorName = "Silver flip/flop";
		return ImColor(137, 135, 136);
		break;
	}

	case brick_180:
	{
		this->ColorName = "Curry";
		return ImColor(215, 169, 75);
		break;
	}

	case brick_190:
	{
		this->ColorName = "Fire Yellow";
		return ImColor(249, 214, 46);
		break;
	}

	case brick_191:
	{
		this->ColorName = "Flame yellowish orange";
		return ImColor(232, 171, 45);
		break;
	}

	case brick_192:
	{
		this->ColorName = "Reddish brown";
		return ImColor(105, 64, 40);
		break;
	}

	case brick_193:
	{
		this->ColorName = "Flame reddish orange";
		return ImColor(207, 96, 36);
		break;
	}

	case brick_194:
	{
		this->ColorName = "Medium stone grey";
		return ImColor(163, 162, 165);
		break;
	}

	case brick_195:
	{
		this->ColorName = "Royal blue";
		return ImColor(70, 103, 164);
		break;
	}

	case brick_196:
	{
		this->ColorName = "Dark Royal blue";
		return ImColor(35, 71, 139);
		break;
	}

	case brick_198:
	{
		this->ColorName = "Bright reddish lilac";
		return ImColor(142, 66, 133);
		break;
	}

	case brick_199:
	{
		this->ColorName = "Dark stone grey";
		return ImColor(99, 95, 98);
		break;
	}

	case brick_200:
	{
		this->ColorName = "Lemon metalic";
		return ImColor(130, 138, 93);
		break;
	}

	case brick_208:
	{
		this->ColorName = "Light stone grey";
		return ImColor(229, 228, 223);
		break;
	}

	case brick_209:
	{
		this->ColorName = "Dark Curry";
		return ImColor(176, 142, 68);
		break;
	}

	case brick_210:
	{
		this->ColorName = "Faded green";
		return ImColor(112, 149, 120);
		break;
	}

	case brick_211:
	{
		this->ColorName = "Turquoise";
		return ImColor(121, 181, 181);
		break;
	}

	case brick_212:
	{
		this->ColorName = "Light Royal blue";
		return ImColor(159, 195, 233);
		break;
	}

	case brick_213:
	{
		this->ColorName = "Medium Royal blue";
		return ImColor(108, 129, 183);
		break;
	}

	case brick_216:
	{
		this->ColorName = "Rust";
		return ImColor(143, 76, 42);
		break;
	}

	case brick_217:
	{
		this->ColorName = "Brown";
		return ImColor(124, 92, 70);
		break;
	}

	case brick_218:
	{
		this->ColorName = "Reddish lilac";
		return ImColor(150, 112, 159);
		break;
	}

	case brick_219:
	{
		this->ColorName = "Lilac";
		return ImColor(107, 98, 155);
		break;
	}

	case brick_220:
	{
		this->ColorName = "Light lilac";
		return ImColor(167, 169, 206);
		break;
	}

	case brick_221:
	{
		this->ColorName = "Bright purple";
		return ImColor(205, 98, 152);
		break;
	}

	case brick_222:
	{
		this->ColorName = "Light purple";
		return ImColor(228, 173, 200);
		break;
	}

	case brick_223:
	{
		this->ColorName = "Light pink";
		return ImColor(220, 144, 149);
		break;
	}

	case brick_224:
	{
		this->ColorName = "Light brick yellow";
		return ImColor(240, 213, 160);
		break;
	}

	case brick_225:
	{
		this->ColorName = "Warm yellowish orange";
		return ImColor(235, 184, 127);
		break;
	}

	case brick_226:
	{
		this->ColorName = "Cool yellow";
		return ImColor(253, 234, 141);
		break;
	}

	case brick_232:
	{
		this->ColorName = "Dove blue";
		return ImColor(125, 187, 221);
		break;
	}

	case brick_268:
	{
		this->ColorName = "Medium lilac";
		return ImColor(52, 43, 117);
		break;
	}

	case brick_301:
	{
		this->ColorName = "Slime green";
		return ImColor(80, 109, 84);
		break;
	}

	case brick_302:
	{
		this->ColorName = "Smoky grey";
		return ImColor(91, 93, 105);
		break;
	}

	case brick_303:
	{
		this->ColorName = "Dark blue";
		return ImColor(0, 16, 176);
		break;
	}

	case brick_304:
	{
		this->ColorName = "Parsley green";
		return ImColor(44, 101, 29);
		break;
	}

	case brick_305:
	{
		this->ColorName = "Steel blue";
		return ImColor(82, 124, 174);
		break;
	}

	case brick_306:
	{
		this->ColorName = "Storm blue";
		return ImColor(51, 88, 130);
		break;
	}

	case brick_307:
	{
		this->ColorName = "Lapis";
		return ImColor(16, 42, 220);
		break;
	}

	case brick_308:
	{
		this->ColorName = "Dark indigo";
		return ImColor(61, 21, 133);
		break;
	}

	case brick_309:
	{
		this->ColorName = "Sea green";
		return ImColor(52, 142, 64);
		break;
	}

	case brick_310:
	{
		this->ColorName = "Shamrock";
		return ImColor(91, 154, 76);
		break;
	}

	case brick_311:
	{
		this->ColorName = "Fossil";
		return ImColor(159, 161, 172);
		break;
	}

	case brick_312:
	{
		this->ColorName = "Mulberry";
		return ImColor(89, 34, 89);
		break;
	}

	case brick_313:
	{
		this->ColorName = "Forest green";
		return ImColor(31, 128, 29);
		break;
	}

	case brick_314:
	{
		this->ColorName = "Cadet blue";
		return ImColor(159, 173, 192);
		break;
	}

	case brick_315:
	{
		this->ColorName = "Electric blue";
		return ImColor(9, 137, 207);
		break;
	}

	case brick_316:
	{
		this->ColorName = "Eggplant";
		return ImColor(123, 0, 123);
		break;
	}

	case brick_317:
	{
		this->ColorName = "Moss";
		return ImColor(124, 156, 107);
		break;
	}

	case brick_318:
	{
		this->ColorName = "Artichoke";
		return ImColor(138, 171, 133);
		break;
	}

	case brick_319:
	{
		this->ColorName = "Sage green";
		return ImColor(185, 196, 177);
		break;
	}

	case brick_320:
	{
		this->ColorName = "Ghost grey";
		return ImColor(202, 203, 209);
		break;
	}

	case brick_321:
	{
		this->ColorName = "Lilac";
		return ImColor(167, 94, 155);
		break;
	}

	case brick_322:
	{
		this->ColorName = "Plum";
		return ImColor(123, 47, 123);
		break;
	}

	case brick_323:
	{
		this->ColorName = "Olivine";
		return ImColor(148, 190, 129);
		break;
	}

	case brick_324:
	{
		this->ColorName = "Laurel green";
		return ImColor(168, 189, 153);
		break;
	}

	case brick_325:
	{
		this->ColorName = "Quill grey";
		return ImColor(223, 223, 222);
		break;
	}

	case brick_327:
	{
		this->ColorName = "Crimson";
		return ImColor(151, 0, 0);
		break;
	}

	case brick_328:
	{
		this->ColorName = "Mint";
		return ImColor(177, 229, 166);
		break;
	}

	case brick_329:
	{
		this->ColorName = "Baby blue";
		return ImColor(152, 194, 219);
		break;
	}

	case brick_330:
	{
		this->ColorName = "Carnation pink";
		return ImColor(255, 152, 220);
		break;
	}

	case brick_331:
	{
		this->ColorName = "Persimmon";
		return ImColor(255, 89, 89);
		break;
	}

	case brick_332:
	{
		this->ColorName = "Maroon";
		return ImColor(117, 0, 0);
		break;
	}

	case brick_333:
	{
		this->ColorName = "Gold";
		return ImColor(239, 184, 56);
		break;
	}

	case brick_334:
	{
		this->ColorName = "Daisy orange";
		return ImColor(248, 217, 109);
		break;
	}

	case brick_335:
	{
		this->ColorName = "Pearl";
		return ImColor(231, 231, 236);
		break;
	}

	case brick_336:
	{
		this->ColorName = "Fog";
		return ImColor(199, 212, 228);
		break;
	}

	case brick_337:
	{
		this->ColorName = "Salmon";
		return ImColor(255, 148, 148);
		break;
	}

	case brick_338:
	{
		this->ColorName = "Terra Cotta";
		return ImColor(190, 104, 98);
		break;
	}

	case brick_339:
	{
		this->ColorName = "Cocoa";
		return ImColor(86, 36, 36);
		break;
	}

	case brick_340:
	{
		this->ColorName = "Wheat";
		return ImColor(241, 231, 199);
		break;
	}

	case brick_341:
	{
		this->ColorName = "Buttermilk";
		return ImColor(254, 243, 187);
		break;
	}

	case brick_342:
	{
		this->ColorName = "Mauve";
		return ImColor(224, 178, 208);
		break;
	}

	case brick_343:
	{
		this->ColorName = "Sunrise";
		return ImColor(212, 144, 189);
		break;
	}

	case brick_344:
	{
		this->ColorName = "Tawny";
		return ImColor(150, 85, 85);
		break;
	}

	case brick_345:
	{
		this->ColorName = "Rust";
		return ImColor(143, 76, 42);
		break;
	}

	case brick_346:
	{
		this->ColorName = "Cashmere";
		return ImColor(211, 190, 150);
		break;
	}

	case brick_347:
	{
		this->ColorName = "Khaki";
		return ImColor(226, 220, 188);
		break;
	}

	case brick_348:
	{
		this->ColorName = "Lily white";
		return ImColor(237, 234, 234);
		break;
	}

	case brick_349:
	{
		this->ColorName = "Seashell";
		return ImColor(233, 218, 218);
		break;
	}

	case brick_350:
	{
		this->ColorName = "Burgundy";
		return ImColor(136, 62, 62);
		break;
	}

	case brick_351:
	{
		this->ColorName = "Cork";
		return ImColor(188, 155, 93);
		break;
	}

	case brick_352:
	{
		this->ColorName = "Burlap";
		return ImColor(199, 172, 120);
		break;
	}

	case brick_353:
	{
		this->ColorName = "Beige";
		return ImColor(202, 191, 163);
		break;
	}

	case brick_354:
	{
		this->ColorName = "Oyster";
		return ImColor(187, 179, 178);
		break;
	}

	case brick_355:
	{
		this->ColorName = "Pine Cone";
		return ImColor(108, 88, 75);
		break;
	}

	case brick_356:
	{
		this->ColorName = "Fawn brown";
		return ImColor(160, 132, 79);
		break;
	}

	case brick_357:
	{
		this->ColorName = "Hurricane grey";
		return ImColor(149, 137, 136);
		break;
	}

	case brick_358:
	{
		this->ColorName = "Cloudy grey";
		return ImColor(171, 168, 158);
		break;
	}

	case brick_359:
	{
		this->ColorName = "Linen";
		return ImColor(175, 148, 131);
		break;
	}

	case brick_360:
	{
		this->ColorName = "Copper";
		return ImColor(150, 103, 102);
		break;
	}

	case brick_361:
	{
		this->ColorName = "Dirt brown";
		return ImColor(86, 66, 54);
		break;
	}

	case brick_362:
	{
		this->ColorName = "Bronze";
		return ImColor(126, 104, 63);
		break;
	}

	case brick_363:
	{
		this->ColorName = "Flint";
		return ImColor(105, 102, 92);
		break;
	}


	case brick_364:
	{
		this->ColorName = "Dark taupe";
		return ImColor(90, 76, 66);
		break;
	}

	case brick_365:
	{
		this->ColorName = "Burnt Sienna";
		return ImColor(106, 57, 9);
		break;
	}

	case roblox_1001:
	{
		this->ColorName = "Institutional white";
		return ImColor(248, 248, 248);
		break;
	}

	case roblox_1002:
	{
		this->ColorName = "Mid gray";
		return ImColor(205, 205, 205);
		break;
	}

	case roblox_1003:
	{
		this->ColorName = "Really black";
		return ImColor(17, 17, 17);
		break;
	}

	case roblox_1004:
	{
		this->ColorName = "Really red";
		return ImColor(255, 0, 0);
		break;
	}

	case roblox_1005:
	{
		this->ColorName = "Deep orange";
		return ImColor(255, 175, 0);
		break;
	}

	case roblox_1006:
	{
		this->ColorName = "Alder";
		return ImColor(180, 128, 255);
		break;
	}

	case roblox_1007:
	{
		this->ColorName = "Dusty Rose";
		return ImColor(163, 75, 75);
		break;
	}

	case roblox_1008:
	{
		this->ColorName = "Olive";
		return ImColor(193, 190, 66);
		break;
	}

	case roblox_1009:
	{
		this->ColorName = "New Yeller";
		return ImColor(255, 255, 0);
		break;
	}

	case roblox_1010:
	{
		this->ColorName = "Really blue";
		return ImColor(0, 0, 255);
		break;
	}

	case roblox_1011:
	{
		this->ColorName = "Navy blue";
		return ImColor(0, 32, 96);
		break;
	}

	case roblox_1012:
	{
		this->ColorName = "Deep blue";
		return ImColor(33, 84, 185);
		break;
	}

	case roblox_1013:
	{
		this->ColorName = "Cyan";
		return ImColor(4, 175, 236);
		break;
	}

	case roblox_1014:
	{
		this->ColorName = "CGA brown";
		return ImColor(170, 85, 0);
		break;
	}

	case roblox_1015:
	{
		this->ColorName = "Magenta";
		return ImColor(170, 0, 170);
		break;
	}

	case roblox_1016:
	{
		this->ColorName = "Pink";
		return ImColor(255, 102, 204);
		break;
	}

	case roblox_1017:
	{
		this->ColorName = "Deep orange";
		return ImColor(255, 175, 0);
		break;
	}

	case roblox_1018:
	{
		this->ColorName = "Teal";
		return ImColor(18, 238, 212);
		break;
	}

	case roblox_1019:
	{
		this->ColorName = "Toothpaste";
		return ImColor(0, 255, 255);
		break;
	}

	case roblox_1020:
	{
		this->ColorName = "Lime green";
		return ImColor(0, 255, 0);
		break;
	}

	case roblox_1021:
	{
		this->ColorName = "Camo";
		return ImColor(58, 125, 21);
		break;
	}

	case roblox_1022:
	{
		this->ColorName = "Grime";
		return ImColor(127, 142, 100);
		break;
	}

	case roblox_1023:
	{
		this->ColorName = "Lavender";
		return ImColor(140, 91, 159);
		break;
	}

	case roblox_1024:
	{
		this->ColorName = "Pastel light blue";
		return ImColor(175, 221, 255);
		break;
	}

	case roblox_1025:
	{
		this->ColorName = "Pastel orange";
		return ImColor(255, 201, 201);
		break;
	}

	case roblox_1026:
	{
		this->ColorName = "Pastel violet";
		return ImColor(177, 167, 255);
		break;
	}

	case roblox_1027:
	{
		this->ColorName = "Pastel blue-green";
		return ImColor(159, 243, 233);
		break;
	}

	case roblox_1028:
	{
		this->ColorName = "Pastel green";
		return ImColor(204, 255, 204);
		break;
	}

	case roblox_1029:
	{
		this->ColorName = "Pastel yellow";
		return ImColor(255, 255, 204);
		break;
	}

	case roblox_1030:
	{
		this->ColorName = "Pastel brown";
		return ImColor(255, 204, 153);
		break;
	}

	case roblox_1031:
	{
		this->ColorName = "Royal purple";
		return ImColor(98, 37, 209);
		break;
	}

	case roblox_1032:
	{
		this->ColorName = "Hot pink";
		return ImColor(255, 0, 191);
		break;
	}

	default:
	{
		this->ColorName = "Neutral";
		return ImColor(255, 255, 255);
		break;
	}
	}
}
