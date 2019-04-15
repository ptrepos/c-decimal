/**
* Copyright (C) Takuo Hada 2015-2016
* @author t.hada 2016/07/09
*
* 256 bit integer for decimal.
* this source includes decimal implementation.
*/
#include <magica/arch/uint256.h>

MG_PRIVATE void mg_uint256_test_to_string(const mg_uint256_t *value, char *buf)
{
	int index;
	mg_uint256_t v, tmp;
	const mg_uint256_t *v10 = mg_uint256_get_10eN(1);

	v = *value;

	index = 0;
	do {
		mg_uint256_div(&v, v10, &tmp);

		int64_t s = mg_uint256_get_int64(&v);
		buf[index++] = (char)(s + '0');

		v = tmp;
	} while(!mg_uint256_is_zero(&v));

	for(int i = 0; i < index / 2; i++) {
		char c = buf[i];
		buf[i] = buf[index - i - 1];
		buf[index - i - 1] = c;
	}
	buf[index] = 0;
}

MG_PRIVATE void mg_uint256_test_convert(const char *buf, mg_uint256_t *value)
{
	mg_uint256_t v, n;
	const mg_uint256_t *v10 = mg_uint256_get_10eN(1);

	mg_uint256_set_zero(&v);

	int i = 0;
	while(buf[i] != 0) {
		int overflow = mg_uint256_mul(&v, v10);
		assert(overflow == 0);

		mg_uint256_set(&n, buf[i] - '0');
		mg_uint256_add(&v, &n);

		i++;
	}
	*value = v;
}

MG_PRIVATE void mg_uint256_test_to_hex_string(const mg_uint256_t *value, char *buf)
{
	int index;
	mg_uint256_t v, tmp;
	mg_uint256_t v16;

	mg_uint256_set(&v16, 16);

	v = *value;

	index = 0;
	do {
		mg_uint256_div(&v, &v16, &tmp);

		int64_t s = mg_uint256_get_int64(&v);
		buf[index++] = s < 10 ? (char)(s + '0'): (char)(s + 'A' - 10);

		v = tmp;
	} while(!mg_uint256_is_zero(&v));

	for(int i = 0; i < index / 2; i++) {
		char c = buf[i];
		buf[i] = buf[index - i - 1];
		buf[index - i - 1] = c;
	}
	buf[index] = 0;
}

MG_PRIVATE void mg_uint256_test_hex_convert(const char *buf, mg_uint256_t *value)
{
	mg_uint256_t v, n;
	mg_uint256_t v16;

	mg_uint256_set(&v16, 16);

	mg_uint256_set_zero(&v);

	int i = 0;
	while(buf[i] != 0) {
		int overflow = mg_uint256_mul(&v, &v16);
		assert(overflow == 0);

		mg_uint256_set(&n, buf[i] >= 'A' ? buf[i] - 'A' + 10: buf[i] - '0');
		mg_uint256_add(&v, &n);

		i++;
	}
	*value = v;
}
