/**
 * Copyright (C) Takuo Hada 2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#include <magica/arch/uint256.h>

static const mg_uint256_t V_10e0 = {0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e1 = {0x0000000a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e2 = {0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e3 = {0x000003e8, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e4 = {0x00002710, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e5 = {0x000186a0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e6 = {0x000f4240, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e7 = {0x00989680, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e8 = {0x05f5e100, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e9 = {0x3b9aca00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e10 = {0x540be400, 0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e11 = {0x4876e800, 0x00000017, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e12 = {0xd4a51000, 0x000000e8, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e13 = {0x4e72a000, 0x00000918, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e14 = {0x107a4000, 0x00005af3, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e15 = {0xa4c68000, 0x00038d7e, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e16 = {0x6fc10000, 0x002386f2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e17 = {0x5d8a0000, 0x01634578, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e18 = {0xa7640000, 0x0de0b6b3, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e19 = {0x89e80000, 0x8ac72304, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e20 = {0x63100000, 0x6bc75e2d, 0x00000005, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e21 = {0xdea00000, 0x35c9adc5, 0x00000036, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e22 = {0xb2400000, 0x19e0c9ba, 0x0000021e, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e23 = {0xf6800000, 0x02c7e14a, 0x0000152d, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e24 = {0xa1000000, 0x1bcecced, 0x0000d3c2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e25 = {0x4a000000, 0x16140148, 0x00084595, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e26 = {0xe4000000, 0xdcc80cd2, 0x0052b7d2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e27 = {0xe8000000, 0x9fd0803c, 0x033b2e3c, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e28 = {0x10000000, 0x3e250261, 0x204fce5e, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e29 = {0xa0000000, 0x6d7217ca, 0x431e0fae, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e30 = {0x40000000, 0x4674edea, 0x9f2c9cd0, 0x0000000c, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e31 = {0x80000000, 0xc0914b26, 0x37be2022, 0x0000007e, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e32 = {0x00000000, 0x85acef81, 0x2d6d415b, 0x000004ee, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e33 = {0x00000000, 0x38c15b0a, 0xc6448d93, 0x0000314d, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e34 = {0x00000000, 0x378d8e64, 0xbead87c0, 0x0001ed09, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e35 = {0x00000000, 0x2b878fe8, 0x72c74d82, 0x00134261, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e36 = {0x00000000, 0xb34b9f10, 0x7bc90715, 0x00c097ce, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e37 = {0x00000000, 0x00f436a0, 0xd5da46d9, 0x0785ee10, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e38 = {0x00000000, 0x098a2240, 0x5a86c47a, 0x4b3b4ca8, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e39 = {0x00000000, 0x5f655680, 0x8943acc4, 0xf050fe93, 0x00000002, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e40 = {0x00000000, 0xb9f56100, 0x5ca4bfab, 0x6329f1c3, 0x0000001d, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e41 = {0x00000000, 0x4395ca00, 0x9e6f7cb5, 0xdfa371a1, 0x00000125, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e42 = {0x00000000, 0xa3d9e400, 0x305adf14, 0xbc627050, 0x00000b7a, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e43 = {0x00000000, 0x6682e800, 0xe38cb6ce, 0x5bd86321, 0x000072cb, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e44 = {0x00000000, 0x011d1000, 0xe37f2410, 0x9673df52, 0x00047bf1, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e45 = {0x00000000, 0x0b22a000, 0xe2f768a0, 0xe086b93c, 0x002cd76f, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e46 = {0x00000000, 0x6f5a4000, 0xddaa1640, 0xc5433c60, 0x01c06a5e, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e47 = {0x00000000, 0x59868000, 0xa8a4de84, 0xb4a05bc8, 0x118427b3, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e48 = {0x00000000, 0x7f410000, 0x9670b12b, 0x0e4395d6, 0xaf298d05, 0x00000000, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e49 = {0x00000000, 0xf88a0000, 0xe066ebb2, 0x8ea3da61, 0xd79f8232, 0x00000006, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e50 = {0x00000000, 0xb5640000, 0xc40534fd, 0x926687d2, 0x6c3b15f9, 0x00000044, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e51 = {0x00000000, 0x15e80000, 0xa83411e9, 0xb8014e3b, 0x3a4edbbf, 0x000002ac, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e52 = {0x00000000, 0xdb100000, 0x9208b31a, 0x300d0e54, 0x4714957d, 0x00001aba, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e53 = {0x00000000, 0x8ea00000, 0xb456ff0c, 0xe0828f4d, 0xc6cdd6e3, 0x00010b46, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e54 = {0x00000000, 0x92400000, 0x0b65f67d, 0xc5199909, 0xc40a64e6, 0x000a70c3, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e55 = {0x00000000, 0xb6800000, 0x71fba0e7, 0xb2fffa5a, 0xa867f103, 0x006867a5, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e56 = {0x00000000, 0x21000000, 0x73d4490d, 0xfdffc788, 0x940f6a24, 0x04140c78, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e57 = {0x00000000, 0x4a000000, 0x864ada83, 0xebfdcb54, 0xc89a2571, 0x28c87cb5, 0x00000000, 0x00000000};
static const mg_uint256_t V_10e58 = {0x00000000, 0xe4000000, 0x3eec8920, 0x37e9f14d, 0xd6057673, 0x97d4df19, 0x00000001, 0x00000000};
static const mg_uint256_t V_10e59 = {0x00000000, 0xe8000000, 0x753d5b48, 0x2f236d04, 0x5c36a080, 0xee50b702, 0x0000000f, 0x00000000};
static const mg_uint256_t V_10e60 = {0x00000000, 0x10000000, 0x946590d9, 0xd762422c, 0x9a224501, 0x4f272617, 0x0000009f, 0x00000000};
static const mg_uint256_t V_10e61 = {0x00000000, 0xa0000000, 0xcbf7a87a, 0x69d695bd, 0x0556b212, 0x17877cec, 0x00000639, 0x00000000};
static const mg_uint256_t V_10e62 = {0x00000000, 0x40000000, 0xf7ac94ca, 0x2261d969, 0x3562f4b8, 0xeb4ae138, 0x00003e3a, 0x00000000};
static const mg_uint256_t V_10e63 = {0x00000000, 0x80000000, 0xacbdcfe6, 0x57d27e23, 0x15dd8f31, 0x30eccc32, 0x00026e4d, 0x00000000};
static const mg_uint256_t V_10e64 = {0x00000000, 0x00000000, 0xbf6a1f01, 0x6e38ed64, 0xdaa797ed, 0xe93ff9f4, 0x00184f03, 0x00000000 };
static const mg_uint256_t V_10e65 = {0x00000000, 0x00000000, 0x7a25360a, 0x4e3945ef, 0x8a8bef46, 0x1c7fc390, 0x00f31627, 0x00000000 };
static const mg_uint256_t V_10e66 = {0x00000000, 0x00000000, 0xc5741c64, 0x0e3cbb5a, 0x697758bf, 0x1cfda3a5, 0x097edd87, 0x00000000 };
static const mg_uint256_t V_10e67 = {0x00000000, 0x00000000, 0xb6891be8, 0x8e5f518b, 0x1ea97776, 0x21e86476, 0x5ef4a747, 0x00000000 };
static const mg_uint256_t V_10e68 = {0x00000000, 0x00000000, 0x215b1710, 0x8fb92f75, 0x329eaaa1, 0x5313ec9d, 0xb58e88c7, 0x00000003 };
static const mg_uint256_t V_10e69 = {0x00000000, 0x00000000, 0x4d8ee6a0, 0x9d3bda93, 0xfa32aa4f, 0x3ec73e23, 0x179157c9, 0x00000025 };
static const mg_uint256_t V_10e70 = {0x00000000, 0x00000000, 0x07950240, 0x245689c1, 0xc5faa71c, 0x73c86d67, 0xebad6ddc, 0x00000172 };
static const mg_uint256_t V_10e71 = {0x00000000, 0x00000000, 0x4bd21680, 0x6b61618a, 0xbbca8719, 0x85d4460d, 0x34c64a9c, 0x00000e7d };
static const mg_uint256_t V_10e72 = {0x00000000, 0x00000000, 0xf634e100, 0x31cdcf66, 0x55e946fe, 0x3a4abc89, 0x0fbeea1d, 0x000090e4 };
static const mg_uint256_t V_10e73 = {0x00000000, 0x00000000, 0x9e10ca00, 0xf20a1a05, 0x5b1cc5ed, 0x46eb5d5d, 0x9d752524, 0x0005a8e8 };
static const mg_uint256_t V_10e74 = {0x00000000, 0x00000000, 0x2ca7e400, 0x74650438, 0x8f1fbb4b, 0xc531a5a5, 0x2693736a, 0x00389916 };
static const mg_uint256_t V_10e75 = {0x00000000, 0x00000000, 0xbe8ee800, 0x8bf22a31, 0x973d50f2, 0xb3f07877, 0x81c2822b, 0x0235fadd };
static const mg_uint256_t V_10e76 = {0x00000000, 0x00000000, 0x71951000, 0x7775a5f1, 0xe8652979, 0x0764b4ab, 0x119915b5, 0x161bcca7 };
static const mg_uint256_t V_10e77 = {0x00000000, 0x00000000, 0x6fd2a000, 0xaa987b6e, 0x13f39ebe, 0x49ef0eb7, 0xaffad912, 0xdd15fe86 };

MG_PRIVATE const mg_uint256_t *MG_UINT256_10eN_TABLE [] = {
	&V_10e0, &V_10e1, &V_10e2, &V_10e3, &V_10e4, &V_10e5, &V_10e6, &V_10e7,
	&V_10e8, &V_10e9, &V_10e10, &V_10e11, &V_10e12, &V_10e13, &V_10e14, &V_10e15,
	&V_10e16, &V_10e17, &V_10e18, &V_10e19, &V_10e20, &V_10e21, &V_10e22, &V_10e23,
	&V_10e24, &V_10e25, &V_10e26, &V_10e27, &V_10e28, &V_10e29, &V_10e30, &V_10e31,
	&V_10e32, &V_10e33, &V_10e34, &V_10e35, &V_10e36, &V_10e37, &V_10e38, &V_10e39,
	&V_10e40, &V_10e41, &V_10e42, &V_10e43, &V_10e44, &V_10e45, &V_10e46, &V_10e47,
	&V_10e48, &V_10e49, &V_10e50, &V_10e51, &V_10e52, &V_10e53, &V_10e54, &V_10e55,
	&V_10e56, &V_10e57, &V_10e58, &V_10e59, &V_10e60, &V_10e61, &V_10e62, &V_10e63,
	&V_10e64, &V_10e65, &V_10e66, &V_10e67, &V_10e68, &V_10e69, &V_10e70, &V_10e71,
	&V_10e72, &V_10e73, &V_10e74, &V_10e75, &V_10e76, &V_10e77,
};

MG_PRIVATE int mg_uint256_get_digits(const mg_uint256_t *value)
{
	int digits = 0;

	assert(value != NULL);

	if (mg_uint256_is_zero(value)) {
		return 0;
	}

	assert(mg_uint256_compare(value, MG_UINT256_10eN_TABLE[77]) <= 0);

	int high = 77;
	int low = 0;
	while(low < high) {
		int middle = (low + high) / 2;
		int cmp = mg_uint256_compare(value, MG_UINT256_10eN_TABLE[middle]);
		if(cmp < 0) {
			high = middle - 1;
		} else if(cmp > 0) {
			low = middle + 1;
		} else {
			low = middle;
			break;
		}
	}

	if(mg_uint256_compare(value, MG_UINT256_10eN_TABLE[low]) >= 0)
		return low + 1;
	return low;
}

#define DOUBLE_RSHIFT_32		(1.0/4294967296.0)
#define DOUBLE_LSHIFT_32		(4294967296.0)
#define DOUBLE_CORRECT			(0.9999)

static inline void set_double(mg_uint256_t *op1, double value, int n)
{
	mg_uint256_set_zero(op1);
	if (n == 0) {
		op1->word[n] = (uint32_t) value;
	} else if(n == 1) {
		op1->word[n] = (uint32_t) value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 1] = (uint32_t)value;
	} else if(n == 2) {
		op1->word[n] = (uint32_t) value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 1] = (uint32_t)value;
		value -= op1->word[n - 1];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 2] = (uint32_t)value;
	} else {
		op1->word[n] = (uint32_t) value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 1] = (uint32_t)value;
		value -= op1->word[n - 1];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 2] = (uint32_t)value;
		value -= op1->word[n - 2];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 3] = (uint32_t)value;
	}
}

MG_PRIVATE mg_error_t mg_uint256_div(mg_uint256_t *op1, const mg_uint256_t *op2, mg_uint256_t *quotient)
{
	mg_error_t err = 0;
	double op1_v, op2_v, q_tmp;
	mg_uint256_t buf1, buf2, buf3;
	mg_uint256_t *q = &buf1, *qv = &buf2, *qv_hi = &buf3;
	int q_n;
	
	int op1_digits = mgUINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;
	int op2_digits = mgUINT256_WORD_COUNT;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	if(op2_digits <= 1) {
		if(op2_digits <= 0) {
			err = mgE_ZERODIVIDE;
			goto _ERROR;
		}

		if(op1_digits <= 1) {
			mg_uint256_set(quotient, op1->word[0] / op2->word[0]);
			mg_uint256_set(op1, op1->word[0] % op2->word[0]);
			goto _EXIT;
		}
	}

	op2_v = (double)op2->word[op2_digits -1];
	if(op2_digits >= 2)
		op2_v += (double)op2->word[op2_digits-2] * DOUBLE_RSHIFT_32;
	if(op2_digits >= 3)
		op2_v += (double)op2->word[op2_digits-3] * DOUBLE_RSHIFT_32 * DOUBLE_RSHIFT_32;

	mg_uint256_set_zero(quotient);

	while (op1_digits >= op2_digits && mg_uint256_compare(op1, op2) >= 0) {
		if(op2_digits <= 1 && op1_digits <= 1) {
			mg_uint256_set(/*out*/q, op1->word[0] / op2->word[0]);
			mg_uint256_set(/*out*/op1, op1->word[0] % op2->word[0]);

			mg_uint256_add(/*out*/quotient, q);
			goto _EXIT;
		}

		op1_v = (double)op1->word[op1_digits-1];
		if(op1_digits >= 2)
			op1_v += (double)op1->word[op1_digits-2] * DOUBLE_RSHIFT_32;

		if(op1_digits >= 3)
			op1_v += (double)op2->word[op1_digits-3] * DOUBLE_RSHIFT_32 * DOUBLE_RSHIFT_32;

		q_n = op1_digits - op2_digits;
		q_tmp = op1_v / op2_v;
		if((q_tmp < 1.0 && q_n == 0)) {
			q_tmp = 1.0;
		} else if(q_tmp >= DOUBLE_LSHIFT_32) {
			q_tmp *= DOUBLE_CORRECT;
		}
		set_double(q, q_tmp, q_n);

		bool overflow = mg_uint256_mul_digits(
			op2, op2_digits, q, q_n + 1, /*out*/qv);

		while(overflow || mg_uint256_compare(op1, qv) < 0) {
			q_tmp *= DOUBLE_CORRECT;
			set_double(q, q_tmp, q_n);

			overflow = mg_uint256_mul_digits(
				op2, op2_digits, q, q_n + 1, /*out*/qv);
		}

		int borrow = mg_uint256_sub(op1, qv);
		assert(borrow == 0);
		mg_uint256_add(quotient, q);

		while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
			op1_digits--;
	}
_EXIT:
	return 0;
_ERROR:
	return err;
}
