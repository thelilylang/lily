#!/bin/python3

# MIT License
#
# Copyright (c) 2022-2025 ArthurPV
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from dataclasses import dataclass
from enum import Enum, auto

import re

class Structure(Enum):
    Struct = auto()
    Enum = auto()

@dataclass
class EnumDebugGenerator:
    variants : str

    def __parse_variants(self) -> list[str]:
        return list(map(lambda x: re.sub(r"\s+", "", x), list(filter(lambda x: x != ' ', self.variants.split(",")))))

    def generate_body(self) -> str:
        parsed_variants = self.__parse_variants()
        res = "switch (self) {\n"

        for p_variant in parsed_variants:
            res += f"\tcase {p_variant}:\n"
            res += f"\t\treturn \"{p_variant}\";\n"

        res += "\tdefault:\n"
        res += "\t\tUNREACHABLE(\"unknown variant\");\n"
        res += "}"

        return res

@dataclass
class StructDebugGenerator:
    fields : str
    struct_name : str

    def __parse_fields(self) -> list[{"name": str, "type": str}]:
        res = []

        for field in self.fields.split(";"):
            field = field.split(' ')
            name = re.sub(r"\s+", "", field[1])
            dt = re.sub(r"\s+", "", field[0])
            re.sub(r"[^a-zA-Z][0-9]", "", dt)
            res.append({"name": name, "type": dt})

        return res

    def generate_body(self) -> str:
        parsed_fields = self.__parse_fields()
        res = f"return format(\"{self.struct_name}{{{{\n"

        for p_field in parsed_fields:
            name = p_field["name"]
            res += f"{name} = "
            res += "{Sr},\\n"

        res += "}\", "

        for p_field in parsed_fields:
            name = p_field["name"]
            dt = p_field["type"]
            res += f"to_string__Debug__{dt}(self->{name})),"

        res = res[:-1] + ");"

        return res

def read_file(filename : str) -> str:
    with open(filename, "r") as f:
        return f.read()

if __name__ == "__main__":
    answer_t = input("What kind of type do you generate a debug (enum/struct)? ")

    if answer_t == "enum":
        filename = input("Please enter the name of the file that contains all the variants you want to generate ")
        variants = read_file(filename)
        enum_dbg_generator = EnumDebugGenerator(variants)
        print(enum_dbg_generator.generate_body())
    elif answer_t == "struct":
        struct_name = input("Please enter the name of the struct ")
        filename = input("Please enter the name of the file that contains all the fields you want to generate ")
        fields = read_file(filename)
        struct_dbg_generator = StructDebugGenerator(fields, struct_name)
        print(struct_dbg_generator.generate_body())
    else:
        print("unknown kind of type")
        exit(1)
