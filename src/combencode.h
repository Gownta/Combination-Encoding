#ifndef GOWNTA_COMBENCODE_H
#define GOWNTA_COMBENCODE_H

/**
 * Encode a sequence.
 *
 * Given a pointer to k sorted (ascending) integers, each between 1 and n,
 * return an integer in [0, C(n,k)-1] that represents that sequence.
 *
 * @param n Upper bound on the values in the sequence.
 * @param k Number of values in the sequence.
 * @param data Sequence of integers to encode.
 * @param method How to perform the encoding - used only for testing.
 * @return A unique encoding of the sequence.
 */
int encode(int n, int k, const int * data, int method);

/**
 * Decode an encoding.
 *
 * Given an encoding e between 0 and C(n,k)-1, store the sequence it
 * represents into the given data pointer.
 *
 * @param n Upper bound on values in the sequence.
 * @param k Number of values in the sequence.
 * @param e Encoding of the sequence.
 * @param method How to perform the decode search - used only for testing.
 * @param data Location to store the values of the sequence.
 */
void decode(int n, int k, int e, int * data, int method);

#endif // GOWNTA_COMBENCODE_H

