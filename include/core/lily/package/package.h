/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_CORE_LILY_PACKAGE_H
#define LILY_CORE_LILY_PACKAGE_H

enum LilyPackageStatus
{
    LILY_PACKAGE_STATUS_LIB_MAIN,
    LILY_PACKAGE_STATUS_MAIN, // Must contain the main function
    LILY_PACKAGE_STATUS_NORMAL,
    LILY_PACKAGE_STATUS_SUB_MAIN,
    LILY_PACKAGE_STATUS_IND // Independent package e.g. a single file
};

/* LilyPackage<T> */
#define LilyPackage(T)                                                         \
    typedef struct LilyPackage__##T                                            \
    {                                                                          \
        String *name;                    /* String* | String* (&) */           \
        String *global_name;             /* String* | String* (&) */           \
        Vec *public_macros;              /* Vec<LilyMacro*>*? */               \
        Vec *private_macros;             /* Vec<LilyMacro*>* */                \
        Vec *public_imports;             /* Vec<LilyImport*>* */               \
        Vec *private_imports;            /* Vec<LilyImport*>* */               \
        Vec *sub_packages;               /* Vec<LilyPackage*>* */              \
        Vec *package_dependencies;       /* Vec<LilyPackage* (&)>* */          \
        Vec *lib_dependencies;           /* Vec<LilyLibrary* (&)>* */          \
        const LilyPackageConfig *config; /* LilyPackageConfig* (&) */          \
                                                                               \
        File file;                                                             \
        LilyScanner scanner;     /* LilyScanner */                             \
        LilyPreparser preparser; /* LilyPreparser */                           \
        LilyPreparserInfo preparser_info;                                      \
        LilyPrecompiler precompiler; /* LilyPrecompiler */                     \
        LilyParser parser;                                                     \
        LilyAnalysis analysis;                                                 \
        LilyMirModule mir_module;                                              \
        LilyBuiltinFun *builtins; /* LilyBuiltinFun*? (&) */                   \
        LilySysFun *syss;         /* LilySysFun*? (&) */                       \
        Usize count_error;                                                     \
        Usize count_warning;                                                   \
        /* count all errors and warnings after the precompiler step */         \
        enum LilyVisibility visibility;                                        \
        enum LilyPackageStatus status;                                         \
                                                                               \
        bool sys_is_loaded;                                                    \
        bool std_is_loaded;                                                    \
        bool core_is_loaded;                                                   \
        bool builtin_is_loaded;                                                \
                                                                               \
        bool main_is_found;                                                    \
                                                                               \
        Vec *builtin_usage; /* Vec<LilyBuiltinFun* (&)>* */                    \
        Vec *sys_usage;     /* Vec<LilySysFun* (&)>* */                        \
                                                                               \
        LilyCheckedOperatorRegister operator_register;                         \
                                                                               \
        const LilyProgram *program;                                            \
                                                                               \
        T adapter;                                                             \
    } LilyPackage__##T;                                                        \
                                                                               \
    /**                                                                        \
     *                                                                         \
     * @brief Construct LilyPackage<T> type.                                   \
     * @param root LilyPackage<T>*?                                            \
     */                                                                        \
    CONSTRUCTOR(LilyPackage__##T *,                                            \
                LilyPackage__##T,                                              \
                String *name,                                                  \
                String *global_name,                                           \
                enum LilyVisibility visibility,                                \
                char *filename,                                                \
                enum LilyPackageStatus status,                                 \
                const char *default_path,                                      \
                const char *default_package_acccess,                           \
                LilyPackage__##T *root);                                       \
                                                                               \
    /**                                                                        \
     *                                                                         \
     * @brief Look for the name of the file among all the packages and return  \
     * the File.                                                               \
     */                                                                        \
    const File *get_file_from_filename__LilyPackage__##T(                      \
      const LilyPackage__##T *self, const char *filename);                     \
                                                                               \
    /**                                                                        \
     *                                                                         \
     * @brief Look for the name of the file among all the packages and return  \
     * the LilyPackage<T>.                                                     \
     */                                                                        \
    LilyPackage__##T *search_package_from_filename__LilyPackage__##T(          \
      LilyPackage__##T *self, const char *filename);                           \
                                                                               \
    /**                                                                        \
     *                                                                         \
     * @brief Search for the package from the name and return LilyPackage<T>   \
     * if found otherwise return NULL.                                         \
     * @return LilyPackage<T>*?                                                \
     */                                                                        \
    LilyPackage__##T *search_package_from_name__LilyPackage__##T(              \
      LilyPackage__##T *self, String *name);                                   \
                                                                               \
    /**                                                                        \
     *                                                                         \
     * @brief Try to add a used builtin function to the register of uses of    \
     * builtin functions.                                                      \
     * @note There is no duplication of builtin functions in the function      \
     * register of the system used.                                            \
     */                                                                        \
    void add_builtin_fun_to_builtin_usage__LilyPackage__##T(                   \
      LilyPackage__##T *self, LilyBuiltinFun *fun_builtin);                    \
                                                                               \
    /**                                                                        \
     *                                                                         \
     * @brief Try to add a used sys function to the register of uses of system \
     * functions.                                                              \
     * @note There is no duplication of system functions in the function       \
     * register of the system used.                                            \
     */                                                                        \
    void add_sys_fun_to_sys_usage__LilyPackage__##T(LilyPackage__##T *self,    \
                                                    LilySysFun *fun_sys);

#define IMPL_LilyPackage(T)                                                  \
    const File *get_file_from_filename__LilyPackage__##T(                    \
      const LilyPackage__##T *self, const char *filename)                    \
    {                                                                        \
        if (!strcmp(filename, self->file.name)) {                            \
            return &self->file;                                              \
        }                                                                    \
                                                                             \
        for (Usize i = 0; i < self->sub_packages->len; i++) {                \
            const File *f = get_file_from_filename__LilyPackage__##T(        \
              get__Vec(self->sub_packages, i), filename);                    \
                                                                             \
            if (f) {                                                         \
                return f;                                                    \
            }                                                                \
        }                                                                    \
                                                                             \
        return NULL;                                                         \
    }                                                                        \
                                                                             \
    LilyPackage__##T *search_package_from_filename__LilyPackage__##T(        \
      LilyPackage__##T *self, const char *filename)                          \
    {                                                                        \
        if (!strcmp(filename, self->file.name)) {                            \
            return self;                                                     \
        }                                                                    \
                                                                             \
        for (Usize i = 0; i < self->sub_packages->len; i++) {                \
            LilyPackage__##T *pkg =                                          \
              search_package_from_filename__LilyPackage__##T(                \
                get__Vec(self->sub_packages, i), filename);                  \
                                                                             \
            if (pkg) {                                                       \
                return pkg;                                                  \
            }                                                                \
        }                                                                    \
                                                                             \
        return NULL;                                                         \
    }                                                                        \
                                                                             \
    LilyPackage__##T *search_package_from_name__LilyPackage__##T(            \
      LilyPackage__##T *self, String *name)                                  \
    {                                                                        \
        if (!strcmp(name->buffer, self->name->buffer)) {                     \
            return self;                                                     \
        }                                                                    \
                                                                             \
        for (Usize i = 0; i < self->sub_packages->len; i++) {                \
            LilyPackage *pkg = search_package_from_name__LilyPackage(        \
              get__Vec(self->sub_packages, i), name);                        \
                                                                             \
            if (pkg) {                                                       \
                return pkg;                                                  \
            }                                                                \
        }                                                                    \
                                                                             \
        return NULL;                                                         \
    }                                                                        \
                                                                             \
    void add_builtin_fun_to_builtin_usage__LilyPackage__##T(                 \
      LilyPackage__##T *self, LilyBuiltinFun *fun_builtin)                   \
    {                                                                        \
        for (Usize i = 0; i < self->builtin_usage->len; ++i) {               \
            if (!strcmp(CAST(LilySysFun *, get__Vec(self->builtin_usage, i)) \
                          ->real_name->buffer,                               \
                        fun_builtin->real_name->buffer)) {                   \
                return;                                                      \
            }                                                                \
        }                                                                    \
                                                                             \
        push__Vec(self->builtin_usage, fun_builtin);                         \
    }                                                                        \
                                                                             \
    void add_sys_fun_to_sys_usage__LilyPackage__##T(LilyPackage__##T *self,  \
                                                    LilySysFun *fun_sys)     \
    {                                                                        \
        for (Usize i = 0; i < self->sys_usage->len; ++i) {                   \
            if (!strcmp(                                                     \
                  CAST(LilySysFun *, get__Vec(self->sys_usage, i))->name,    \
                  fun_sys->name)) {                                          \
                return;                                                      \
            }                                                                \
        }                                                                    \
                                                                             \
        push__Vec(self->sys_usage, fun_sys);                                 \
    }

#endif // LILY_CORE_LILY_PACKAGE_H
