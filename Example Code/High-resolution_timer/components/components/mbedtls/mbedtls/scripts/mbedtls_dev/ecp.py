"""Framework classes for generation of ecp test cases."""
# Copyright The Mbed TLS Contributors
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from typing import List

from . import test_data_generation
from . import bignum_common


class EcpTarget(test_data_generation.BaseTarget):
    #pylint: disable=abstract-method, too-few-public-methods
    """Target for ecp test case generation."""
    target_basename = 'test_suite_ecp.generated'


class EcpP192R1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P192 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p192_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP192R1_ENABLED",
                    "MBEDTLS_ECP_NIST_OPTIM"]

    moduli = ["fffffffffffffffffffffffffffffffeffffffffffffffff"] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        "fffffffffffffffffffffffffffffffefffffffffffffffe",

        # Modulus + 1
        "ffffffffffffffffffffffffffffffff0000000000000000",

        # 2^192 - 1
        "ffffffffffffffffffffffffffffffffffffffffffffffff",

        # Maximum canonical P192 multiplication result
        ("fffffffffffffffffffffffffffffffdfffffffffffffffc"
         "000000000000000100000000000000040000000000000004"),

        # Generate an overflow during reduction
        ("00000000000000000000000000000001ffffffffffffffff"
         "ffffffffffffffffffffffffffffffff0000000000000000"),

        # Generate an overflow during carry reduction
        ("ffffffffffffffff00000000000000010000000000000000"
         "fffffffffffffffeffffffffffffffff0000000000000000"),

        # First 8 number generated by random.getrandbits(384) - seed(2,2)
        ("cf1822ffbc6887782b491044d5e341245c6e433715ba2bdd"
         "177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("ffed9235288bc781ae66267594c9c9500925e4749b575bd1"
         "3653f8dd9b1f282e4067c3584ee207f8da94e3e8ab73738f"),
        ("ef8acd128b4f2fc15f3f57ebf30b94fa82523e86feac7eb7"
         "dc38f519b91751dacdbd47d364be8049a372db8f6e405d93"),
        ("e8624fab5186ee32ee8d7ee9770348a05d300cb90706a045"
         "defc044a09325626e6b58de744ab6cce80877b6f71e1f6d2"),
        ("2d3d854e061b90303b08c6e33c7295782d6c797f8f7d9b78"
         "2a1be9cd8697bbd0e2520e33e44c50556c71c4a66148a86f"),
        ("fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f1"
         "5c14bc4a829e07b0829a48d422fe99a22c70501e533c9135"),
        ("97eeab64ca2ce6bc5d3fd983c34c769fe89204e2e8168561"
         "867e5e15bc01bfce6a27e0dfcbf8754472154e76e4c11ab2"),
        ("bd143fa9b714210c665d7435c1066932f4767f26294365b2"
         "721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b"),

        # Next 2 number generated by random.getrandbits(192)
        "47733e847d718d733ff98ff387c56473a7a83ee0761ebfd2",
        "cbd4d3e2d4dec9ef83f0be4e80371eb97f81375eecc1cb63"
    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self)-> List[str]:
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP192R1"] + args


class EcpP224R1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P224 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p224_raw"
    input_style = "arch_split"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP224R1_ENABLED",
                    "MBEDTLS_ECP_NIST_OPTIM"]

    moduli = ["ffffffffffffffffffffffffffffffff000000000000000000000001"] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        "ffffffffffffffffffffffffffffffff000000000000000000000000",

        # Modulus + 1
        "ffffffffffffffffffffffffffffffff000000000000000000000002",

        # 2^224 - 1
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff",

        # Maximum canonical P224 multiplication result
        ("fffffffffffffffffffffffffffffffe000000000000000000000000"
         "00000001000000000000000000000000000000000000000000000000"),

        # Generate an overflow during reduction
        ("00000000000000000000000000010000000070000000002000001000"
         "ffffffffffff9fffffffffe00000efff000070000000002000001003"),

        # Generate an underflow during reduction
        ("00000001000000000000000000000000000000000000000000000000"
         "00000000000dc0000000000000000001000000010000000100000003"),

        # First 8 number generated by random.getrandbits(448) - seed(2,2)
        ("da94e3e8ab73738fcf1822ffbc6887782b491044d5e341245c6e4337"
         "15ba2bdd177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("cdbd47d364be8049a372db8f6e405d93ffed9235288bc781ae662675"
         "94c9c9500925e4749b575bd13653f8dd9b1f282e4067c3584ee207f8"),
        ("defc044a09325626e6b58de744ab6cce80877b6f71e1f6d2ef8acd12"
         "8b4f2fc15f3f57ebf30b94fa82523e86feac7eb7dc38f519b91751da"),
        ("2d6c797f8f7d9b782a1be9cd8697bbd0e2520e33e44c50556c71c4a6"
         "6148a86fe8624fab5186ee32ee8d7ee9770348a05d300cb90706a045"),
        ("8f54f8ceacaab39e83844b40ffa9b9f15c14bc4a829e07b0829a48d4"
         "22fe99a22c70501e533c91352d3d854e061b90303b08c6e33c729578"),
        ("97eeab64ca2ce6bc5d3fd983c34c769fe89204e2e8168561867e5e15"
         "bc01bfce6a27e0dfcbf8754472154e76e4c11ab2fec3f6b32e8d4b8a"),
        ("a7a83ee0761ebfd2bd143fa9b714210c665d7435c1066932f4767f26"
         "294365b2721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b"),
        ("74667bffe202849da9643a295a9ac6decbd4d3e2d4dec9ef83f0be4e"
         "80371eb97f81375eecc1cb6347733e847d718d733ff98ff387c56473"),

        # Next 2 number generated by random.getrandbits(224)
        "eb9ac688b9d39cca91551e8259cc60b17604e4b4e73695c3e652c71a",
        "f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f258ebdbfe3"
    ]

    @property
    def arg_a(self) -> str:
        limbs = 2 * bignum_common.bits_to_limbs(224, self.bits_in_limb)
        hex_digits = bignum_common.hex_digits_for_limb(limbs, self.bits_in_limb)
        return super().format_arg('{:x}'.format(self.int_a)).zfill(hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self)-> List[str]:
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP224R1"] + args


class EcpP256R1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P256 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p256_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP256R1_ENABLED",
                    "MBEDTLS_ECP_NIST_OPTIM"]

    moduli = ["ffffffff00000001000000000000000000000000ffffffffffffffffffffffff"] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        "ffffffff00000001000000000000000000000000fffffffffffffffffffffffe",

        # Modulus + 1
        "ffffffff00000001000000000000000000000001000000000000000000000000",

        # 2^256 - 1
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",

        # Maximum canonical P256 multiplication result
        ("fffffffe00000002fffffffe0000000100000001fffffffe00000001fffffffc"
         "00000003fffffffcfffffffffffffffffffffffc000000000000000000000004"),

        # Generate an overflow during reduction
        ("0000000000000000000000010000000000000000000000000000000000000000"
         "00000000000000000000000000000000000000000000000000000000ffffffff"),

        # Generate an underflow during reduction
        ("0000000000000000000000000000000000000000000000000000000000000010"
         "ffffffff00000000000000000000000000000000000000000000000000000000"),

        # Generate an overflow during carry reduction
        ("aaaaaaaa00000000000000000000000000000000000000000000000000000000"
         "00000000000000000000000000000000aaaaaaacaaaaaaaaaaaaaaaa00000000"),

        # Generate an underflow during carry reduction
        ("000000000000000000000001ffffffff00000000000000000000000000000000"
         "0000000000000000000000000000000000000002000000020000000100000002"),

        # First 8 number generated by random.getrandbits(512) - seed(2,2)
        ("4067c3584ee207f8da94e3e8ab73738fcf1822ffbc6887782b491044d5e34124"
         "5c6e433715ba2bdd177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("82523e86feac7eb7dc38f519b91751dacdbd47d364be8049a372db8f6e405d93"
         "ffed9235288bc781ae66267594c9c9500925e4749b575bd13653f8dd9b1f282e"),
        ("e8624fab5186ee32ee8d7ee9770348a05d300cb90706a045defc044a09325626"
         "e6b58de744ab6cce80877b6f71e1f6d2ef8acd128b4f2fc15f3f57ebf30b94fa"),
        ("829a48d422fe99a22c70501e533c91352d3d854e061b90303b08c6e33c729578"
         "2d6c797f8f7d9b782a1be9cd8697bbd0e2520e33e44c50556c71c4a66148a86f"),
        ("e89204e2e8168561867e5e15bc01bfce6a27e0dfcbf8754472154e76e4c11ab2"
         "fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f15c14bc4a829e07b0"),
        ("bd143fa9b714210c665d7435c1066932f4767f26294365b2721dea3bf63f23d0"
         "dbe53fcafb2147df5ca495fa5a91c89b97eeab64ca2ce6bc5d3fd983c34c769f"),
        ("74667bffe202849da9643a295a9ac6decbd4d3e2d4dec9ef83f0be4e80371eb9"
         "7f81375eecc1cb6347733e847d718d733ff98ff387c56473a7a83ee0761ebfd2"),
        ("d08f1bb2531d6460f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f25"
         "8ebdbfe3eb9ac688b9d39cca91551e8259cc60b17604e4b4e73695c3e652c71a"),

        # Next 2 number generated by random.getrandbits(256)
        "c5e2486c44a4a8f69dc8db48e86ec9c6e06f291b2a838af8d5c44a4eb3172062",
        "d4c0dca8b4c9e755cc9c3adcf515a8234da4daeb4f3f87777ad1f45ae9500ec9"
    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self)-> List[str]:
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP256R1"] + args


class EcpP384R1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P384 fast reduction."""
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p384_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP384R1_ENABLED",
                    "MBEDTLS_ECP_NIST_OPTIM"]

    moduli = [("ffffffffffffffffffffffffffffffffffffffffffffffff"
               "fffffffffffffffeffffffff0000000000000000ffffffff")
             ] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        ("ffffffffffffffffffffffffffffffffffffffffffffffff"
         "fffffffffffffffeffffffff0000000000000000fffffffe"),

        # Modulus + 1
        ("ffffffffffffffffffffffffffffffffffffffffffffffff"
         "fffffffffffffffeffffffff000000000000000100000000"),

        # 2^384 - 1
        ("ffffffffffffffffffffffffffffffffffffffffffffffff"
         "ffffffffffffffffffffffffffffffffffffffffffffffff"),

        # Maximum canonical P384 multiplication result
        ("ffffffffffffffffffffffffffffffffffffffffffffffff"
         "fffffffffffffffdfffffffe0000000000000001fffffffc"
         "000000000000000000000000000000010000000200000000"
         "fffffffe000000020000000400000000fffffffc00000004"),

        # Testing with overflow in A(12) + A(21) + A(20);
        ("497811378624857a2c2af60d70583376545484cfae5c812f"
         "e2999fc1abb51d18b559e8ca3b50aaf263fdf8f24bdfb98f"
         "ffffffff20e65bf9099e4e73a5e8b517cf4fbeb8fd1750fd"
         "ae6d43f2e53f82d5ffffffffffffffffcc6f1e06111c62e0"),

        # Testing with underflow in A(13) + A(22) + A(23) - A(12) - A(20);
        ("dfdd25e96777406b3c04b8c7b406f5fcf287e1e576003a09"
         "2852a6fbe517f2712b68abef41dbd35183a0614fb7222606"
         "ffffffff84396eee542f18a9189d94396c784059c17a9f18"
         "f807214ef32f2f10ffffffff8a77fac20000000000000000"),

        # Testing with overflow in A(23) + A(20) + A(19) - A(22);
        ("783753f8a5afba6c1862eead1deb2fcdd907272be3ffd185"
         "42b24a71ee8b26cab0aa33513610ff973042bbe1637cc9fc"
         "99ad36c7f703514572cf4f5c3044469a8f5be6312c19e5d3"
         "f8fc1ac6ffffffffffffffff8c86252400000000ffffffff"),

        # Testing with underflow in A(23) + A(20) + A(19) - A(22);
        ("65e1d2362fce922663b7fd517586e88842a9b4bd092e93e6"
         "251c9c69f278cbf8285d99ae3b53da5ba36e56701e2b17c2"
         "25f1239556c5f00117fa140218b46ebd8e34f50d0018701f"
         "a8a0a5cc00000000000000004410bcb4ffffffff00000000"),

        # Testing the second round of carry reduction
        ("000000000000000000000000ffffffffffffffffffffffff"
         "ffffffffffffffffffffffffffffffff0000000000000000"
         "0000000000000000ffffffff000000000000000000000001"
         "00000000000000000000000000000000ffffffff00000001"),

        # First 8 number generated by random.getrandbits(768) - seed(2,2)
        ("ffed9235288bc781ae66267594c9c9500925e4749b575bd1"
         "3653f8dd9b1f282e4067c3584ee207f8da94e3e8ab73738f"
         "cf1822ffbc6887782b491044d5e341245c6e433715ba2bdd"
         "177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("e8624fab5186ee32ee8d7ee9770348a05d300cb90706a045"
         "defc044a09325626e6b58de744ab6cce80877b6f71e1f6d2"
         "ef8acd128b4f2fc15f3f57ebf30b94fa82523e86feac7eb7"
         "dc38f519b91751dacdbd47d364be8049a372db8f6e405d93"),
        ("fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f1"
         "5c14bc4a829e07b0829a48d422fe99a22c70501e533c9135"
         "2d3d854e061b90303b08c6e33c7295782d6c797f8f7d9b78"
         "2a1be9cd8697bbd0e2520e33e44c50556c71c4a66148a86f"),
        ("bd143fa9b714210c665d7435c1066932f4767f26294365b2"
         "721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b"
         "97eeab64ca2ce6bc5d3fd983c34c769fe89204e2e8168561"
         "867e5e15bc01bfce6a27e0dfcbf8754472154e76e4c11ab2"),
        ("8ebdbfe3eb9ac688b9d39cca91551e8259cc60b17604e4b4"
         "e73695c3e652c71a74667bffe202849da9643a295a9ac6de"
         "cbd4d3e2d4dec9ef83f0be4e80371eb97f81375eecc1cb63"
         "47733e847d718d733ff98ff387c56473a7a83ee0761ebfd2"),
        ("d4c0dca8b4c9e755cc9c3adcf515a8234da4daeb4f3f8777"
         "7ad1f45ae9500ec9c5e2486c44a4a8f69dc8db48e86ec9c6"
         "e06f291b2a838af8d5c44a4eb3172062d08f1bb2531d6460"
         "f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f25"),
        ("0227eeb7b9d7d01f5769da05d205bbfcc8c69069134bccd3"
         "e1cf4f589f8e4ce0af29d115ef24bd625dd961e6830b54fa"
         "7d28f93435339774bb1e386c4fd5079e681b8f5896838b76"
         "9da59b74a6c3181c81e220df848b1df78feb994a81167346"),
        ("d322a7353ead4efe440e2b4fda9c025a22f1a83185b98f5f"
         "c11e60de1b343f52ea748db9e020307aaeb6db2c3a038a70"
         "9779ac1f45e9dd320c855fdfa7251af0930cdbd30f0ad2a8"
         "1b2d19a2beaa14a7ff3fe32a30ffc4eed0a7bd04e85bfcdd"),

        # Next 2 number generated by random.getrandbits(384)
        ("5c3747465cc36c270e8a35b10828d569c268a20eb78ac332"
         "e5e138e26c4454b90f756132e16dce72f18e859835e1f291"),
        ("eb2b5693babb7fbb0a76c196067cfdcb11457d9cf45e2fa0"
         "1d7f4275153924800600571fac3a5b263fdf57cd2c006497")
    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self)-> List[str]:
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP384R1"] + args


class EcpP521R1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P521 fast reduction."""
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p521_raw"
    input_style = "arch_split"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP521R1_ENABLED",
                    "MBEDTLS_ECP_NIST_OPTIM"]

    moduli = [("01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
               "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")
             ] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        ("01ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe"),

        # Modulus + 1
        ("020000000000000000000000000000000000000000000000000000000000000000"
         "000000000000000000000000000000000000000000000000000000000000000000"),

        # Maximum canonical P521 multiplication result
        ("0003ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "fffff800"
         "0000000000000000000000000000000000000000000000000000000000000000"
         "0000000000000000000000000000000000000000000000000000000000000004"),

        # Test case for overflow during addition
        ("0001efffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "000001ef"
         "0000000000000000000000000000000000000000000000000000000000000000"
         "000000000000000000000000000000000000000000000000000000000f000000"),

        # First 8 number generated by random.getrandbits(1042) - seed(2,2)
        ("0003cc2e82523e86feac7eb7dc38f519b91751dacdbd47d364be8049a372db8f"
         "6e405d93ffed9235288bc781ae66267594c9c9500925e4749b575bd13653f8dd"
         "9b1f282e"
         "4067c3584ee207f8da94e3e8ab73738fcf1822ffbc6887782b491044d5e34124"
         "5c6e433715ba2bdd177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("00017052829e07b0829a48d422fe99a22c70501e533c91352d3d854e061b9030"
         "3b08c6e33c7295782d6c797f8f7d9b782a1be9cd8697bbd0e2520e33e44c5055"
         "6c71c4a6"
         "6148a86fe8624fab5186ee32ee8d7ee9770348a05d300cb90706a045defc044a"
         "09325626e6b58de744ab6cce80877b6f71e1f6d2ef8acd128b4f2fc15f3f57eb"),
        ("00021f15a7a83ee0761ebfd2bd143fa9b714210c665d7435c1066932f4767f26"
         "294365b2721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b97eeab64"
         "ca2ce6bc"
         "5d3fd983c34c769fe89204e2e8168561867e5e15bc01bfce6a27e0dfcbf87544"
         "72154e76e4c11ab2fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f1"),
        ("000381bc2a838af8d5c44a4eb3172062d08f1bb2531d6460f0caeef038c89b38"
         "a8acb5137c9260dc74e088a9b9492f258ebdbfe3eb9ac688b9d39cca91551e82"
         "59cc60b1"
         "7604e4b4e73695c3e652c71a74667bffe202849da9643a295a9ac6decbd4d3e2"
         "d4dec9ef83f0be4e80371eb97f81375eecc1cb6347733e847d718d733ff98ff3"),
        ("00034816c8c69069134bccd3e1cf4f589f8e4ce0af29d115ef24bd625dd961e6"
         "830b54fa7d28f93435339774bb1e386c4fd5079e681b8f5896838b769da59b74"
         "a6c3181c"
         "81e220df848b1df78feb994a81167346d4c0dca8b4c9e755cc9c3adcf515a823"
         "4da4daeb4f3f87777ad1f45ae9500ec9c5e2486c44a4a8f69dc8db48e86ec9c6"),
        ("000397846c4454b90f756132e16dce72f18e859835e1f291d322a7353ead4efe"
         "440e2b4fda9c025a22f1a83185b98f5fc11e60de1b343f52ea748db9e020307a"
         "aeb6db2c"
         "3a038a709779ac1f45e9dd320c855fdfa7251af0930cdbd30f0ad2a81b2d19a2"
         "beaa14a7ff3fe32a30ffc4eed0a7bd04e85bfcdd0227eeb7b9d7d01f5769da05"),
        ("00002c3296e6bc4d62b47204007ee4fab105d83e85e951862f0981aebc1b00d9"
         "2838e766ef9b6bf2d037fe2e20b6a8464174e75a5f834da70569c018eb2b5693"
         "babb7fbb"
         "0a76c196067cfdcb11457d9cf45e2fa01d7f4275153924800600571fac3a5b26"
         "3fdf57cd2c0064975c3747465cc36c270e8a35b10828d569c268a20eb78ac332"),
        ("00009d23b4917fc09f20dbb0dcc93f0e66dfe717c17313394391b6e2e6eacb0f"
         "0bb7be72bd6d25009aeb7fa0c4169b148d2f527e72daf0a54ef25c0707e33868"
         "7d1f7157"
         "5653a45c49390aa51cf5192bbf67da14be11d56ba0b4a2969d8055a9f03f2d71"
         "581d8e830112ff0f0948eccaf8877acf26c377c13f719726fd70bddacb4deeec"),

        # Next 2 number generated by random.getrandbits(521)
        ("12b84ae65e920a63ac1f2b64df6dff07870c9d531ae72a47403063238da1a1fe"
         "3f9d6a179fa50f96cd4aff9261aa92c0e6f17ec940639bc2ccdf572df00790813e3"),
        ("166049dd332a73fa0b26b75196cf87eb8a09b27ec714307c68c425424a1574f1"
         "eedf5b0f16cdfdb839424d201e653f53d6883ca1c107ca6e706649889c0c7f38608")
    ]

    @property
    def arg_a(self) -> str:
        # Number of limbs: 2 * N
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self)-> List[str]:
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP521R1"] + args


class EcpP192K1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P192K1 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p192k1_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP192K1_ENABLED"]

    moduli = ["fffffffffffffffffffffffffffffffffffffffeffffee37"] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        "fffffffffffffffffffffffffffffffffffffffeffffee36",

        # Modulus + 1
        "fffffffffffffffffffffffffffffffffffffffeffffee38",

        # 2^192 - 1
        "ffffffffffffffffffffffffffffffffffffffffffffffff",

        # Maximum canonical P192K1 multiplication result
        ("fffffffffffffffffffffffffffffffffffffffdffffdc6c"
         "0000000000000000000000000000000100002394013c7364"),

        # Test case for overflow during addition
        ("00000007ffff71b809e27dd832cfd5e04d9d2dbb9f8da217"
         "0000000000000000000000000000000000000000520834f0"),

        # First 8 number generated by random.getrandbits(384) - seed(2,2)
        ("cf1822ffbc6887782b491044d5e341245c6e433715ba2bdd"
         "177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("ffed9235288bc781ae66267594c9c9500925e4749b575bd1"
         "3653f8dd9b1f282e4067c3584ee207f8da94e3e8ab73738f"),
        ("ef8acd128b4f2fc15f3f57ebf30b94fa82523e86feac7eb7"
         "dc38f519b91751dacdbd47d364be8049a372db8f6e405d93"),
        ("e8624fab5186ee32ee8d7ee9770348a05d300cb90706a045"
         "defc044a09325626e6b58de744ab6cce80877b6f71e1f6d2"),
        ("2d3d854e061b90303b08c6e33c7295782d6c797f8f7d9b78"
         "2a1be9cd8697bbd0e2520e33e44c50556c71c4a66148a86f"),
        ("fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f1"
         "5c14bc4a829e07b0829a48d422fe99a22c70501e533c9135"),
        ("97eeab64ca2ce6bc5d3fd983c34c769fe89204e2e8168561"
         "867e5e15bc01bfce6a27e0dfcbf8754472154e76e4c11ab2"),
        ("bd143fa9b714210c665d7435c1066932f4767f26294365b2"
         "721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b"),

        # Next 2 number generated by random.getrandbits(192)
        "47733e847d718d733ff98ff387c56473a7a83ee0761ebfd2",
        "cbd4d3e2d4dec9ef83f0be4e80371eb97f81375eecc1cb63"
    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self):
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP192K1"] + args


class EcpP224K1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P224 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p224k1_raw"
    input_style = "arch_split"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP224K1_ENABLED"]

    moduli = ["fffffffffffffffffffffffffffffffffffffffffffffffeffffe56d"] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        "fffffffffffffffffffffffffffffffffffffffffffffffeffffe56c",

        # Modulus + 1
        "fffffffffffffffffffffffffffffffffffffffffffffffeffffe56e",

        # 2^224 - 1
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff",

        # Maximum canonical P224K1 multiplication result
        ("fffffffffffffffffffffffffffffffffffffffffffffffdffffcad8"
         "00000000000000000000000000000000000000010000352802c26590"),

        # Test case for overflow during addition
        ("0000007ffff2b68161180fd8cd92e1a109be158a19a99b1809db8032"
         "0000000000000000000000000000000000000000000000000bf04f49"),

        # First 8 number generated by random.getrandbits(448) - seed(2,2)
        ("da94e3e8ab73738fcf1822ffbc6887782b491044d5e341245c6e4337"
         "15ba2bdd177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("cdbd47d364be8049a372db8f6e405d93ffed9235288bc781ae662675"
         "94c9c9500925e4749b575bd13653f8dd9b1f282e4067c3584ee207f8"),
        ("defc044a09325626e6b58de744ab6cce80877b6f71e1f6d2ef8acd12"
         "8b4f2fc15f3f57ebf30b94fa82523e86feac7eb7dc38f519b91751da"),
        ("2d6c797f8f7d9b782a1be9cd8697bbd0e2520e33e44c50556c71c4a6"
         "6148a86fe8624fab5186ee32ee8d7ee9770348a05d300cb90706a045"),
        ("8f54f8ceacaab39e83844b40ffa9b9f15c14bc4a829e07b0829a48d4"
         "22fe99a22c70501e533c91352d3d854e061b90303b08c6e33c729578"),
        ("97eeab64ca2ce6bc5d3fd983c34c769fe89204e2e8168561867e5e15"
         "bc01bfce6a27e0dfcbf8754472154e76e4c11ab2fec3f6b32e8d4b8a"),
        ("a7a83ee0761ebfd2bd143fa9b714210c665d7435c1066932f4767f26"
         "294365b2721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b"),
        ("74667bffe202849da9643a295a9ac6decbd4d3e2d4dec9ef83f0be4e"
         "80371eb97f81375eecc1cb6347733e847d718d733ff98ff387c56473"),

        # Next 2 number generated by random.getrandbits(224)
        ("eb9ac688b9d39cca91551e8259cc60b17604e4b4e73695c3e652c71a"),
        ("f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f258ebdbfe3"),
    ]

    @property
    def arg_a(self) -> str:
        limbs = 2 * bignum_common.bits_to_limbs(224, self.bits_in_limb)
        hex_digits = bignum_common.hex_digits_for_limb(limbs, self.bits_in_limb)
        return super().format_arg('{:x}'.format(self.int_a)).zfill(hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self):
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP224K1"] + args


class EcpP256K1Raw(bignum_common.ModOperationCommon,
                   EcpTarget):
    """Test cases for ECP P256 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p256k1_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_SECP256K1_ENABLED"]

    moduli = ["fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f"] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2e",

        # Modulus + 1
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc30",

        # 2^256 - 1
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",

        # Maximum canonical P256K1 multiplication result
        ("fffffffffffffffffffffffffffffffffffffffffffffffffffffffdfffff85c"
         "000000000000000000000000000000000000000000000001000007a4000e9844"),

        # Test case for overflow during addition
        ("0000fffffc2f000e90a0c86a0a63234e5ba641f43a7e4aecc4040e67ec850562"
         "00000000000000000000000000000000000000000000000000000000585674fd"),

        # Test case for overflow during addition
        ("0000fffffc2f000e90a0c86a0a63234e5ba641f43a7e4aecc4040e67ec850562"
         "00000000000000000000000000000000000000000000000000000000585674fd"),

        # First 8 number generated by random.getrandbits(512) - seed(2,2)
        ("4067c3584ee207f8da94e3e8ab73738fcf1822ffbc6887782b491044d5e34124"
         "5c6e433715ba2bdd177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("82523e86feac7eb7dc38f519b91751dacdbd47d364be8049a372db8f6e405d93"
         "ffed9235288bc781ae66267594c9c9500925e4749b575bd13653f8dd9b1f282e"),
        ("e8624fab5186ee32ee8d7ee9770348a05d300cb90706a045defc044a09325626"
         "e6b58de744ab6cce80877b6f71e1f6d2ef8acd128b4f2fc15f3f57ebf30b94fa"),
        ("829a48d422fe99a22c70501e533c91352d3d854e061b90303b08c6e33c729578"
         "2d6c797f8f7d9b782a1be9cd8697bbd0e2520e33e44c50556c71c4a66148a86f"),
        ("e89204e2e8168561867e5e15bc01bfce6a27e0dfcbf8754472154e76e4c11ab2"
         "fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f15c14bc4a829e07b0"),
        ("bd143fa9b714210c665d7435c1066932f4767f26294365b2721dea3bf63f23d0"
         "dbe53fcafb2147df5ca495fa5a91c89b97eeab64ca2ce6bc5d3fd983c34c769f"),
        ("74667bffe202849da9643a295a9ac6decbd4d3e2d4dec9ef83f0be4e80371eb9"
         "7f81375eecc1cb6347733e847d718d733ff98ff387c56473a7a83ee0761ebfd2"),
        ("d08f1bb2531d6460f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f25"
         "8ebdbfe3eb9ac688b9d39cca91551e8259cc60b17604e4b4e73695c3e652c71a"),

        # Next 2 number generated by random.getrandbits(256)
        ("c5e2486c44a4a8f69dc8db48e86ec9c6e06f291b2a838af8d5c44a4eb3172062"),
        ("d4c0dca8b4c9e755cc9c3adcf515a8234da4daeb4f3f87777ad1f45ae9500ec9"),
    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self):
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_SECP256K1"] + args


class EcpP255Raw(bignum_common.ModOperationCommon,
                 EcpTarget):
    """Test cases for ECP 25519 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "mbedtls_ecp_mod_p255_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_CURVE25519_ENABLED"]

    moduli = [("7fffffffffffffffffffffffffffffffffffffffffffffffff"
               "ffffffffffffed")] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        ("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffec"),

        # Modulus + 1
        ("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffee"),

        # 2^255 - 1
        ("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),

        # Maximum canonical P255 multiplication result
        ("3fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffec"
         "0000000000000000000000000000000000000000000000000000000000000190"),

        # First 8 number generated by random.getrandbits(510) - seed(2,2)
        ("1019f0d64ee207f8da94e3e8ab73738fcf1822ffbc6887782b491044d5e34124"
         "5c6e433715ba2bdd177219d30e7a269fd95bafc8f2a4d27bdcf4bb99f4bea973"),
        ("20948fa1feac7eb7dc38f519b91751dacdbd47d364be8049a372db8f6e405d93"
         "ffed9235288bc781ae66267594c9c9500925e4749b575bd13653f8dd9b1f282e"),
        ("3a1893ea5186ee32ee8d7ee9770348a05d300cb90706a045defc044a09325626"
         "e6b58de744ab6cce80877b6f71e1f6d2ef8acd128b4f2fc15f3f57ebf30b94fa"),
        ("20a6923522fe99a22c70501e533c91352d3d854e061b90303b08c6e33c729578"
         "2d6c797f8f7d9b782a1be9cd8697bbd0e2520e33e44c50556c71c4a66148a86f"),
        ("3a248138e8168561867e5e15bc01bfce6a27e0dfcbf8754472154e76e4c11ab2"
         "fec3f6b32e8d4b8a8f54f8ceacaab39e83844b40ffa9b9f15c14bc4a829e07b0"),
        ("2f450feab714210c665d7435c1066932f4767f26294365b2721dea3bf63f23d0"
         "dbe53fcafb2147df5ca495fa5a91c89b97eeab64ca2ce6bc5d3fd983c34c769f"),
        ("1d199effe202849da9643a295a9ac6decbd4d3e2d4dec9ef83f0be4e80371eb9"
         "7f81375eecc1cb6347733e847d718d733ff98ff387c56473a7a83ee0761ebfd2"),
        ("3423c6ec531d6460f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f25"
         "8ebdbfe3eb9ac688b9d39cca91551e8259cc60b17604e4b4e73695c3e652c71a"),

        # Next 2 number generated by random.getrandbits(255)
        ("62f1243644a4a8f69dc8db48e86ec9c6e06f291b2a838af8d5c44a4eb3172062"),
        ("6a606e54b4c9e755cc9c3adcf515a8234da4daeb4f3f87777ad1f45ae9500ec9"),
    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self)-> List[str]:
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_CURVE25519"] + args


class EcpP448Raw(bignum_common.ModOperationCommon,
                 EcpTarget):
    """Test cases for ECP P448 fast reduction."""
    symbol = "-"
    test_function = "ecp_mod_p_generic_raw"
    test_name = "ecp_mod_p448_raw"
    input_style = "fixed"
    arity = 1
    dependencies = ["MBEDTLS_ECP_DP_CURVE448_ENABLED"]

    moduli = [("fffffffffffffffffffffffffffffffffffffffffffffffffffffffe"
               "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff")] # type: List[str]

    input_values = [
        "0", "1",

        # Modulus - 1
        ("fffffffffffffffffffffffffffffffffffffffffffffffffffffffe"
         "fffffffffffffffffffffffffffffffffffffffffffffffffffffffe"),

        # Modulus + 1
        ("ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "00000000000000000000000000000000000000000000000000000000"),

        # 2^448 - 1
        ("ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
         "ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"),

        # Maximum canonical P448 multiplication result
        ("fffffffffffffffffffffffffffffffffffffffffffffffffffffffd"
         "fffffffffffffffffffffffffffffffffffffffffffffffffffffffd"
         "00000000000000000000000000000000000000000000000000000004"
         "00000000000000000000000000000000000000000000000000000004"),

        # First 8 number generated by random.getrandbits(896) - seed(2,2)
        ("74667bffe202849da9643a295a9ac6decbd4d3e2d4dec9ef83f0be4e"
         "80371eb97f81375eecc1cb6347733e847d718d733ff98ff387c56473"
         "a7a83ee0761ebfd2bd143fa9b714210c665d7435c1066932f4767f26"
         "294365b2721dea3bf63f23d0dbe53fcafb2147df5ca495fa5a91c89b"),
        ("4da4daeb4f3f87777ad1f45ae9500ec9c5e2486c44a4a8f69dc8db48"
         "e86ec9c6e06f291b2a838af8d5c44a4eb3172062d08f1bb2531d6460"
         "f0caeef038c89b38a8acb5137c9260dc74e088a9b9492f258ebdbfe3"
         "eb9ac688b9d39cca91551e8259cc60b17604e4b4e73695c3e652c71a"),
        ("bc1b00d92838e766ef9b6bf2d037fe2e20b6a8464174e75a5f834da7"
         "0569c018eb2b5693babb7fbb0a76c196067cfdcb11457d9cf45e2fa0"
         "1d7f4275153924800600571fac3a5b263fdf57cd2c0064975c374746"
         "5cc36c270e8a35b10828d569c268a20eb78ac332e5e138e26c4454b9"),
        ("8d2f527e72daf0a54ef25c0707e338687d1f71575653a45c49390aa5"
         "1cf5192bbf67da14be11d56ba0b4a2969d8055a9f03f2d71581d8e83"
         "0112ff0f0948eccaf8877acf26c377c13f719726fd70bddacb4deeec"
         "0b0c995e96e6bc4d62b47204007ee4fab105d83e85e951862f0981ae"),
        ("84ae65e920a63ac1f2b64df6dff07870c9d531ae72a47403063238da"
         "1a1fe3f9d6a179fa50f96cd4aff9261aa92c0e6f17ec940639bc2ccd"
         "f572df00790813e32748dd1db4917fc09f20dbb0dcc93f0e66dfe717"
         "c17313394391b6e2e6eacb0f0bb7be72bd6d25009aeb7fa0c4169b14"),
        ("2bb3b36f29421c4021b7379f0897246a40c270b00e893302aba9e7b8"
         "23fc5ad2f58105748ed5d1b7b310b730049dd332a73fa0b26b75196c"
         "f87eb8a09b27ec714307c68c425424a1574f1eedf5b0f16cdfdb8394"
         "24d201e653f53d6883ca1c107ca6e706649889c0c7f3860895bfa813"),
        ("af3f5d7841b1256d5c1dc12fb5a1ae519fb8883accda6559caa538a0"
         "9fc9370d3a6b86a7975b54a31497024640332b0612d4050771d7b14e"
         "b6c004cc3b8367dc3f2bb31efe9934ad0809eae3ef232a32b5459d83"
         "fbc46f1aea990e94821d46063b4dbf2ca294523d74115c86188b1044"),
        ("7430051376e31f5aab63ad02854efa600641b4fa37a47ce41aeffafc"
         "3b45402ac02659fe2e87d4150511baeb198ababb1a16daff3da95cd2"
         "167b75dfb948f82a8317cba01c75f67e290535d868a24b7f627f2855"
         "09167d4126af8090013c3273c02c6b9586b4625b475b51096c4ad652"),

        # Corner case which causes maximum overflow
        ("f4ae65e920a63ac1f2b64df6dff07870c9d531ae72a47403063238da1"
         "a1fe3f9d6a179fa50f96cd4aff9261aa92c0e6f17ec940639bc2ccd0B"
         "519A16DF59C53E0D49B209200F878F362ACE518D5B8BFCF9CDC725E5E"
         "01C06295E8605AF06932B5006D9E556D3F190E8136BF9C643D332"),

        # Next 2 number generated by random.getrandbits(448)
        ("8f54f8ceacaab39e83844b40ffa9b9f15c14bc4a829e07b0829a48d4"
         "22fe99a22c70501e533c91352d3d854e061b90303b08c6e33c729578"),
        ("97eeab64ca2ce6bc5d3fd983c34c769fe89204e2e8168561867e5e15"
         "bc01bfce6a27e0dfcbf8754472154e76e4c11ab2fec3f6b32e8d4b8a"),

    ]

    @property
    def arg_a(self) -> str:
        return super().format_arg('{:x}'.format(self.int_a)).zfill(2 * self.hex_digits)

    def result(self) -> List[str]:
        result = self.int_a % self.int_n
        return [self.format_result(result)]

    @property
    def is_valid(self) -> bool:
        return True

    def arguments(self):
        args = super().arguments()
        return  ["MBEDTLS_ECP_DP_CURVE448"] + args
