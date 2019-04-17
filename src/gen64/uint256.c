/**
* Copyright (C) Takuo Hada 2015-2016
* @author t.hada 2016/07/09
*
* 256 bit integer for decimal.
* this source includes decimal implementation.
*/
#include <magica/arch/uint256.h>

static const mg_uint256_t V_10e0 = { 0x1 };
static const mg_uint256_t V_10e1 = { 0xa };
static const mg_uint256_t V_10e2 = { 0x64 };
static const mg_uint256_t V_10e3 = { 0x3e8 };
static const mg_uint256_t V_10e4 = { 0x2710 };
static const mg_uint256_t V_10e5 = { 0x186a0 };
static const mg_uint256_t V_10e6 = { 0xf4240 };
static const mg_uint256_t V_10e7 = { 0x989680 };
static const mg_uint256_t V_10e8 = { 0x5f5e100 };
static const mg_uint256_t V_10e9 = { 0x3b9aca00 };
static const mg_uint256_t V_10e10 = { 0x2540be400 };
static const mg_uint256_t V_10e11 = { 0x174876e800 };
static const mg_uint256_t V_10e12 = { 0xe8d4a51000 };
static const mg_uint256_t V_10e13 = { 0x9184e72a000 };
static const mg_uint256_t V_10e14 = { 0x5af3107a4000 };
static const mg_uint256_t V_10e15 = { 0x38d7ea4c68000 };
static const mg_uint256_t V_10e16 = { 0x2386f26fc10000 };
static const mg_uint256_t V_10e17 = { 0x16345785d8a0000 };
static const mg_uint256_t V_10e18 = { 0xde0b6b3a7640000 };
static const mg_uint256_t V_10e19 = { 0x8ac7230489e80000 };
static const mg_uint256_t V_10e20 = { 0x6bc75e2d63100000, 0x5 };
static const mg_uint256_t V_10e21 = { 0x35c9adc5dea00000, 0x36 };
static const mg_uint256_t V_10e22 = { 0x19e0c9bab2400000, 0x21e };
static const mg_uint256_t V_10e23 = { 0x2c7e14af6800000, 0x152d };
static const mg_uint256_t V_10e24 = { 0x1bcecceda1000000, 0xd3c2 };
static const mg_uint256_t V_10e25 = { 0x161401484a000000, 0x84595 };
static const mg_uint256_t V_10e26 = { 0xdcc80cd2e4000000, 0x52b7d2 };
static const mg_uint256_t V_10e27 = { 0x9fd0803ce8000000, 0x33b2e3c };
static const mg_uint256_t V_10e28 = { 0x3e25026110000000, 0x204fce5e };
static const mg_uint256_t V_10e29 = { 0x6d7217caa0000000, 0x1431e0fae };
static const mg_uint256_t V_10e30 = { 0x4674edea40000000, 0xc9f2c9cd0 };
static const mg_uint256_t V_10e31 = { 0xc0914b2680000000, 0x7e37be2022 };
static const mg_uint256_t V_10e32 = { 0x85acef8100000000, 0x4ee2d6d415b };
static const mg_uint256_t V_10e33 = { 0x38c15b0a00000000, 0x314dc6448d93 };
static const mg_uint256_t V_10e34 = { 0x378d8e6400000000, 0x1ed09bead87c0 };
static const mg_uint256_t V_10e35 = { 0x2b878fe800000000, 0x13426172c74d82 };
static const mg_uint256_t V_10e36 = { 0xb34b9f1000000000, 0xc097ce7bc90715 };
static const mg_uint256_t V_10e37 = { 0xf436a000000000, 0x785ee10d5da46d9 };
static const mg_uint256_t V_10e38 = { 0x98a224000000000, 0x4b3b4ca85a86c47a };
static const mg_uint256_t V_10e39 = { 0x5f65568000000000, 0xf050fe938943acc4, 0x2 };
static const mg_uint256_t V_10e40 = { 0xb9f5610000000000, 0x6329f1c35ca4bfab, 0x1d };
static const mg_uint256_t V_10e41 = { 0x4395ca0000000000, 0xdfa371a19e6f7cb5, 0x125 };
static const mg_uint256_t V_10e42 = { 0xa3d9e40000000000, 0xbc627050305adf14, 0xb7a };
static const mg_uint256_t V_10e43 = { 0x6682e80000000000, 0x5bd86321e38cb6ce, 0x72cb };
static const mg_uint256_t V_10e44 = { 0x11d100000000000, 0x9673df52e37f2410, 0x47bf1 };
static const mg_uint256_t V_10e45 = { 0xb22a00000000000, 0xe086b93ce2f768a0, 0x2cd76f };
static const mg_uint256_t V_10e46 = { 0x6f5a400000000000, 0xc5433c60ddaa1640, 0x1c06a5e };
static const mg_uint256_t V_10e47 = { 0x5986800000000000, 0xb4a05bc8a8a4de84, 0x118427b3 };
static const mg_uint256_t V_10e48 = { 0x7f41000000000000, 0xe4395d69670b12b, 0xaf298d05 };
static const mg_uint256_t V_10e49 = { 0xf88a000000000000, 0x8ea3da61e066ebb2, 0x6d79f8232 };
static const mg_uint256_t V_10e50 = { 0xb564000000000000, 0x926687d2c40534fd, 0x446c3b15f9 };
static const mg_uint256_t V_10e51 = { 0x15e8000000000000, 0xb8014e3ba83411e9, 0x2ac3a4edbbf };
static const mg_uint256_t V_10e52 = { 0xdb10000000000000, 0x300d0e549208b31a, 0x1aba4714957d };
static const mg_uint256_t V_10e53 = { 0x8ea0000000000000, 0xe0828f4db456ff0c, 0x10b46c6cdd6e3 };
static const mg_uint256_t V_10e54 = { 0x9240000000000000, 0xc51999090b65f67d, 0xa70c3c40a64e6 };
static const mg_uint256_t V_10e55 = { 0xb680000000000000, 0xb2fffa5a71fba0e7, 0x6867a5a867f103 };
static const mg_uint256_t V_10e56 = { 0x2100000000000000, 0xfdffc78873d4490d, 0x4140c78940f6a24 };
static const mg_uint256_t V_10e57 = { 0x4a00000000000000, 0xebfdcb54864ada83, 0x28c87cb5c89a2571 };
static const mg_uint256_t V_10e58 = { 0xe400000000000000, 0x37e9f14d3eec8920, 0x97d4df19d6057673, 0x1 };
static const mg_uint256_t V_10e59 = { 0xe800000000000000, 0x2f236d04753d5b48, 0xee50b7025c36a080, 0xf };
static const mg_uint256_t V_10e60 = { 0x1000000000000000, 0xd762422c946590d9, 0x4f2726179a224501, 0x9f };
static const mg_uint256_t V_10e61 = { 0xa000000000000000, 0x69d695bdcbf7a87a, 0x17877cec0556b212, 0x639 };
static const mg_uint256_t V_10e62 = { 0x4000000000000000, 0x2261d969f7ac94ca, 0xeb4ae1383562f4b8, 0x3e3a };
static const mg_uint256_t V_10e63 = { 0x8000000000000000, 0x57d27e23acbdcfe6, 0x30eccc3215dd8f31, 0x26e4d };
static const mg_uint256_t V_10e64 = { 0x0000000000000000, 0x6e38ed64bf6a1f01, 0xe93ff9f4daa797ed, 0x184f03 };
static const mg_uint256_t V_10e65 = { 0x0000000000000000, 0x4e3945ef7a25360a, 0x1c7fc3908a8bef46, 0xf31627 };
static const mg_uint256_t V_10e66 = { 0x0000000000000000, 0x0e3cbb5ac5741c64, 0x1cfda3a5697758bf, 0x97edd87 };
static const mg_uint256_t V_10e67 = { 0x0000000000000000, 0x8e5f518bb6891be8, 0x21e864761ea97776, 0x5ef4a747 };
static const mg_uint256_t V_10e68 = { 0x0000000000000000, 0x8fb92f75215b1710, 0x5313ec9d329eaaa1, 0x3b58e88c7 };
static const mg_uint256_t V_10e69 = { 0x0000000000000000, 0x9d3bda934d8ee6a0, 0x3ec73e23fa32aa4f, 0x25179157c9 };
static const mg_uint256_t V_10e70 = { 0x0000000000000000, 0x245689c107950240, 0x73c86d67c5faa71c, 0x172ebad6ddc };
static const mg_uint256_t V_10e71 = { 0x0000000000000000, 0x6b61618a4bd21680, 0x85d4460dbbca8719, 0xe7d34c64a9c };
static const mg_uint256_t V_10e72 = { 0x0000000000000000, 0x31cdcf66f634e100, 0x3a4abc8955e946fe, 0x90e40fbeea1d };
static const mg_uint256_t V_10e73 = { 0x0000000000000000, 0xf20a1a059e10ca00, 0x46eb5d5d5b1cc5ed, 0x5a8e89d752524 };
static const mg_uint256_t V_10e74 = { 0x0000000000000000, 0x746504382ca7e400, 0xc531a5a58f1fbb4b, 0x3899162693736a };
static const mg_uint256_t V_10e75 = { 0x0000000000000000, 0x8bf22a31be8ee800, 0xb3f07877973d50f2, 0x235fadd81c2822b };
static const mg_uint256_t V_10e76 = { 0x0000000000000000, 0x7775a5f171951000, 0x0764b4abe8652979, 0x161bcca7119915b5 };
static const mg_uint256_t V_10e77 = { 0x0000000000000000, 0xaa987b6e6fd2a000, 0x49ef0eb713f39ebe, 0xdd15fe86affad912 };

MG_PRIVATE const mg_uint256_t *MG_UINT256_10eN_TABLE[] = {
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

	if(mg_uint256_is_zero(value)) {
		return 0;
	}

	//assert(mg_uint256_compare(value, MG_UINT256_10eN_TABLE[77]) <= 0);

	int high = 77;
	int low = 0;
	do {
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
	} while(low < high);

	if(mg_uint256_compare(value, MG_UINT256_10eN_TABLE[low]) >= 0)
		return low + 1;
	return low;
}

// ’·œ–@‚É‚æ‚éŽÀ‘•
MG_PRIVATE mg_error_t mg_uint256_div_long_division(
	mg_uint256_t *op1,
	int op1_digits,
	const mg_uint256_t *op2,
	int op2_digits,
	mg_uint256_t *quotient);

MG_PRIVATE mg_error_t mg_uint256_div(mg_uint256_t *op1, const mg_uint256_t *op2, mg_uint256_t *quotient)
{
	mg_error_t err = 0;

	int op1_digits = mgUINT256_WORD_COUNT;
	while(op1_digits > 0 && op1->word[op1_digits - 1] == 0)
		op1_digits--;

	if(op1_digits <= 0) {
		mg_uint256_set_zero(quotient);
		goto _EXIT;
	}

	int op2_digits = mgUINT256_WORD_COUNT;
	while(op2_digits > 0 && op2->word[op2_digits - 1] == 0)
		op2_digits--;

	if(op2_digits <= 1) {
		if(op2_digits <= 0) {
			err = mgE_ZERODIVIDE;
			goto _ERROR;
		}
		if(op1_digits <= 1) {
			uint64_t q = op1->word[0] / op2->word[0];
			uint64_t r = op1->word[0] % op2->word[0];
			mg_uint256_set(quotient, q);
			mg_uint256_set(op1, r);
			goto _EXIT;
		}
	}

	err = mg_uint256_div_long_division(
		/*inout*/op1, op1_digits,
		op2, op2_digits,
		/*out*/quotient);
	if(err != 0)
		goto _ERROR;
	goto _EXIT;

_EXIT:
	return 0;
_ERROR:
	return err;
}

#define DOUBLE_RSHIFT_64		(5.4210108624275221700372640043497e-20)
#define DOUBLE_LSHIFT_64		(18446744073709551616.0)
#define DOUBLE_CORRECT			(0.99999995)

static inline void set_double(mg_uint256_t *op1, double value, int n)
{
	mg_uint256_set_zero(op1);
	
	//printf("%lf\n", value);
	assert(value <= (double)UINT64_MAX);

	if(n == 0) {
		op1->word[n] = (uint64_t)value;
	} else if(n == 1) {
		op1->word[n] = (uint64_t)value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_64;
		op1->word[n - 1] = (uint64_t)value;
	} else {
		op1->word[n] = (uint64_t)value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_64;
		op1->word[n - 1] = (uint64_t)value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_64;
		op1->word[n - 2] = (uint64_t)value;
	}
}

MG_PRIVATE mg_error_t mg_uint256_div_long_division(
	mg_uint256_t *op1,
	int op1_digits,
	const mg_uint256_t *op2,
	int op2_digits,
	mg_uint256_t *quotient)
{
	mg_uint256_t buf1, buf2, buf3;
	mg_uint256_t *q = &buf1, *qv = &buf2, *qv_hi = &buf3;

	double op2_v = (double)op2->word[op2_digits - 1];
	if(op2_digits >= 2)
		op2_v += (double)op2->word[op2_digits - 2] * DOUBLE_RSHIFT_64;
	double op2_vr = 1.0 / op2_v;

	mg_uint256_set_zero(/*out*/quotient);

	while(op1_digits >= op2_digits && !mg_uint256_less_than(op1, op2)) {
		if(op2_digits <= 1 && op1_digits <= 1) {
			uint64_t iq = op1->word[0] / op2->word[0];
			uint64_t ir = op1->word[0] % op2->word[0];
			mg_uint256_set(op1, ir);
			mg_uint256_add_ull(/*out*/quotient, iq);
			goto _EXIT;
		}

		double op1_v = (double)op1->word[op1_digits - 1];
		if(op1_digits >= 2)
			op1_v += (double)op1->word[op1_digits - 2] * DOUBLE_RSHIFT_64;

		int q_n = op1_digits - op2_digits;
		double q_tmp = op1_v * op2_vr;
		if((q_tmp < 1.0 && q_n == 0)) {
			q_tmp = 1.0;
		} else if(q_tmp >= DOUBLE_LSHIFT_64) {
			q_tmp *= DOUBLE_CORRECT;
		}

		for(;;) {
			set_double(q, q_tmp, q_n);

			int overflow = mg_uint256_mul_digits(
				op2, op2_digits, q, q_n + 1, /*out*/qv);

			if(!(overflow || mg_uint256_less_than(op1, qv))) {
				break;
			}

			q_tmp *= DOUBLE_CORRECT;
		}

		int borrow = mg_uint256_sub(op1, qv);
		assert(borrow == 0);
		mg_uint256_add(quotient, q);

		while(op1_digits > 0 && op1->word[op1_digits - 1] == 0)
			op1_digits--;
	}
_EXIT:
	return 0;
}
