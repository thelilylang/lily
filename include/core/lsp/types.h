/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_CORE_LSP_TYPES_H
#define LILY_CORE_LSP_TYPES_H

/**
 *
 * @version LSP v3.17
 * @url
 * https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/
 */

#include <base/types.h>

#include <stdbool.h>

/**
 * Defines an integer number in the range of -2^31 to 2^31 - 1.
 */
typedef Int32 integer;

/**
 * Defines an unsigned integer number in the range of 0 to 2^31 - 1.
 */
typedef Uint32 uinteger;

/**
 * Defines a decimal number. Since decimal numbers are very
 * rare in the language server specification we denote the
 * exact range with every decimal using the mathematics
 * interval notation (e.g. [0, 1] denotes all decimals d with
 * 0 <= d <= 1.
 */
typedef Float64 decimal;

typedef const char *string;

/**
 * The LSP any type
 *
 * @since 3.17.0
 */
typedef struct LSPAny LSPAny;

/**
 * LSP arrays.
 *
 * @since 3.17.0
 */
typedef struct LSPArray
{
    LSPAny *items;
    Usize len;
} LSPArray;

#define MAP(t, t2) \
    struct         \
    {              \
        t key;     \
        t2 value;  \
    }

#define OPTIONAL(t)   \
    struct            \
    {                 \
        t value;      \
        bool is_some; \
    }

#define OPTIONAL_SOME(t, v)         \
    (OPTIONAL(t))                   \
    {                               \
        .is_some = true, .value = v \
    }
#define OPTIONAL_NONE(t) \
    (OPTIONAL(t))        \
    {                    \
        .is_some = false \
    }

#define OPTIONAL_UNWRAP(o) (o).value

#define ARRAY(t)   \
    struct         \
    {              \
        t *buffer; \
        Usize len; \
    }

#define ARRAY_LEN(a) (a).len
#define ARRAY_GET(a) (a).buffer

/**
 * LSP object definition.
 *
 * @since 3.17.0
 */
typedef struct LSPObject
{
    string key;
    LSPAny *value;
} LSPObject;

enum LSPAnyKind
{
    LSP_ANY_KIND_OBJECT,
    LSP_ANY_KIND_ARRAY,
    LSP_ANY_KIND_STRING,
    LSP_ANY_KIND_INTEGER,
    LSP_ANY_KIND_UINTEGER,
    LSP_ANY_KIND_DECIMAL,
    LSP_ANY_KIND_BOOLEAN,
    LSP_ANY_KIND_NULL
};

struct LSPAny
{
    enum LSPAnyKind kind;
    union
    {
        LSPObject object;
        LSPArray array;
        string string;
        integer integer;
        uinteger uinteger;
        decimal decimal;
        bool boolean;
    };
};

#define Message()       \
    {                   \
        string jsonrpc; \
    }

typedef struct Message
Message() Message;

typedef struct RequestMessage
{
    /**
     * The request id.
     */
    // id: integer | string;
    LSPAny id;

    /**
     * The method to be invoked.
     */
    string method;

    /**
     * The method's params.
     */
    // params?: array | object;
    OPTIONAL(LSPAny) params;

    struct Message();
} RequestMessage;

typedef struct ResponseError
{
    /**
     * A number indicating the error type that occurred.
     */
    integer code;

    /**
     * A string providing a short description of the error.
     */
    string message;

    /**
     * A primitive or structured value that contains additional
     * information about the error. Can be omitted.
     */
    // data?: string | number | boolean | array | object | null;
    OPTIONAL(LSPAny) data;
} ResponseError;

typedef struct ResponseMessage
{
    /**
     * The request id.
     */
    // id: integer | string | null;
    LSPAny id;

    /**
     * The result of a request. This member is REQUIRED on success.
     * This member MUST NOT exist if there was an error invoking the method.
     */
    // result?: string | number | boolean | array | object | null;
    OPTIONAL(LSPAny) result;

    /**
     * The error object in case a request fails.
     */
    // error?: ResponseError;
    OPTIONAL(ResponseError) error;

    struct Message();
} ResponseMessage;

// Defined by JSON-RPC
const integer ParseError__ErrorCodes = -32700;
const integer InvalidRequest__ErrorCodes = -32600;
const integer MethodNotFound__ErrorCodes = -32601;
const integer InvalidParams__ErrorCodes = -32602;
const integer InternalError__ErrorCodes = -32603;

/**
 * This is the start range of JSON-RPC reserved error codes.
 * It doesn't denote a real error code. No LSP error codes should
 * be defined between the start and end range. For backwards
 * compatibility the `ServerNotInitialized` and the `UnknownErrorCode`
 * are left in the range.
 *
 * @since 3.16.0
 */
const integer jsonrpcReservedErrorRangeStart__ErrorCodes = -32099;
/** @deprecated use jsonrpcReservedErrorRangeStart */
const integer serverErrorStart__ErrorCodes =
  jsonrpcReservedErrorRangeStart__ErrorCodes;

/**
 * Error code indicating that a server received a notification or
 * request before the server has received the `initialize` request.
 */
const integer ServerNotInitialized__ErrorCodes = -32002;
const integer UnknownErrorCode__ErrorCodes = -32001;

/**
 * This is the end range of JSON-RPC reserved error codes.
 * It doesn't denote a real error code.
 *
 * @since 3.16.0
 */
const integer jsonrpcReservedErrorRangeEnd__ErrorCodes = -32000;
/** @deprecated use jsonrpcReservedErrorRangeEnd */
const integer serverErrorEnd__ErrorCodes =
  jsonrpcReservedErrorRangeEnd__ErrorCodes;

/**
 * This is the start range of LSP reserved error codes.
 * It doesn't denote a real error code.
 *
 * @since 3.16.0
 */
const integer lspReservedErrorRangeStart__ErrorCodes = -32899;

/**
 * A request failed but it was syntactically correct, e.g the
 * method name was known and the parameters were valid. The error
 * message should contain human readable information about why
 * the request failed.
 *
 * @since 3.17.0
 */
const integer RequestFailed__ErrorCodes = -32803;

/**
 * The server cancelled the request. This error code should
 * only be used for requests that explicitly support being
 * server cancellable.
 *
 * @since 3.17.0
 */
const integer ServerCancelled__ErrorCodes = -32802;

/**
 * The server detected that the content of a document got
 * modified outside normal conditions. A server should
 * NOT send this error code if it detects a content change
 * in it unprocessed messages. The result even computed
 * on an older state might still be useful for the client.
 *
 * If a client decides that a result is not of any use anymore
 * the client should cancel the request.
 */
const integer ContentModified__ErrorCodes = -32801;

/**
 * The client has canceled a request and a server as detected
 * the cancel.
 */
const integer RequestCancelled__ErrorCodes = -32800;

/**
 * This is the end range of LSP reserved error codes.
 * It doesn't denote a real error code.
 *
 * @since 3.16.0
 */
const integer lspReservedErrorRangeEnd__ErrorCodes = -32800;

typedef struct NotificationMessage
{
    /**
     * The method to be invoked.
     */
    string method;

    /**
     * The notification's params.
     */
    // params?: array | object;
    OPTIONAL(LSPAny) params;

    struct Message();
} NotificationMessage;

typedef struct CancelParams
{
    /**
     * The request id to cancel.
     */
    LSPAny id;
} CancelParams;

// type ProgressToken = integer | string;
typedef LSPAny ProgressToken;

#define ProgressParams(T)                                       \
    struct                                                      \
    {                                                           \
        /**                                                     \
         * The progress token provided by the client or server. \
         */                                                     \
        ProgressToken token;                                    \
                                                                \
        /**                                                     \
         * The progress data.                                   \
         */                                                     \
        T value;                                                \
    }

typedef struct HoverParams
{
    string textDocument; /** The text document's URI in string form */
    struct
    {
        uinteger line;
        uinteger character;
    } position;
} HoverParams;

typedef struct HoverResult
{
    string value;
} HoverResult;

typedef string DocumentUri;
typedef string URI;

typedef struct RegularExpressionsClientCapabilities
{
    /**
     * The engine's name.
     */
    string engine;

    /**
     * The engine's version.
     */
    // version?: string;
    OPTIONAL(string) version;
} RegularExpressionsClientCapabilities;

const string EOL[] = { "\n", "\r\n", "\r" };

typedef struct Position
{
    /**
     * Line position in a document (zero-based).
     */
    uinteger line;

    /**
     * Character offset on a line in a document (zero-based). The meaning of
     * this offset is determined by the negotiated `PositionEncodingKind`.
     *
     * If the character value is greater than the line length it defaults back
     * to the line length.
     */
    uinteger character;
} Position;

/**
 * A type indicating how positions are encoded,
 * specifically what column offsets mean.
 *
 * @since 3.17.0
 */
typedef string PositionEncodingKind;

/**
 * Character offsets count UTF-8 code units (e.g bytes).
 */
const PositionEncodingKind UTF8__PositionEncodingKind = "utf-8";

/**
 * Character offsets count UTF-16 code units.
 *
 * This is the default and must always be supported
 * by servers
 */
const PositionEncodingKind UTF16__PositionEncodingKind = "utf-16";

/**
 * Character offsets count UTF-32 code units.
 *
 * Implementation note: these are the same as Unicode code points,
 * so this `PositionEncodingKind` may also be used for an
 * encoding-agnostic representation of character offsets.
 */
const PositionEncodingKind UTF32__PositionEncodingKind = "utf-32";

typedef struct Range
{
    /**
     * The range's start position.
     */
    Position start;

    /**
     * The range's end position.
     */
    Position end;
} Range;

typedef struct TextDocumentItem
{
    /**
     * The text document's URI.
     */
    DocumentUri uri;

    /**
     * The text document's language identifier.
     */
    string languageId;

    /**
     * The version number of this document (it will increase after each
     * change, including undo/redo).
     */
    integer version;

    /**
     * The content of the opened text document.
     */
    string text;
} TextDocumentItem;

typedef struct TextDocumentIdentifier
{
    /**
     * The text document's URI.
     */
    DocumentUri uri;
} TextDocumentIdentifier;

typedef struct VersionedTextDocumentIdentifier
{
    /**
     * The version number of this document.
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     */
    integer version;

    /**
     * The text document's URI.
     */
    DocumentUri uri;
} VersionedTextDocumentIdentifier;

typedef struct OptionalVersionedTextDocumentIdentifier
{
    /**
     * The version number of this document. If an optional versioned text
     * document identifier is sent from the server to the client and the file is
     * not open in the editor (the server has not received an open notification
     * before) the server can send `null` to indicate that the version is
     * known and the content on disk is the master (as specified with document
     * content ownership).
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     */
    // version: integer | null;
    LSPAny version;

    /**
     * The text document's URI.
     */
    DocumentUri uri;
} OptionalVersionedTextDocumentIdentifier;

typedef struct TextDocumentPositionParams
{
    /**
     * The text document.
     */
    TextDocumentIdentifier textDocument;

    /**
     * The position inside the text document.
     */
    Position position;
} TextDocumentPositionParams;

typedef struct DocumentFilter
{
    /**
     * A language id, like `typescript`.
     */
    OPTIONAL(string) language;

    /**
     * A Uri [scheme](#Uri.scheme), like `file` or `untitled`.
     */
    OPTIONAL(string) scheme;

    /**
     * A glob pattern, like `*.{ts,js}`.
     *
     * Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment
     * - `?` to match on one character in a path segment
     * - `**` to match any number of path segments, including none
     * - `{}` to group sub patterns into an OR expression. (e.g.
     * `**​/\*.{ts,js}` matches all TypeScript and JavaScript files)
     * - `[]` to declare a range of characters to match in a path segment
     *   (e.g., `example.[0-9]` to match on `example.0`, `example.1`, …)
     * - `[!...]` to negate a range of characters to match in a path segment
     *   (e.g., `example.[!0-9]` to match on `example.a`, `example.b`, but
     *   not `example.0`)
     */
    OPTIONAL(string) pattern;
} DocumentFilter;

typedef ARRAY(DocumentFilter) DocumentSelector;

#define TextEdit()                                                     \
                                                                       \
    {                                                                  \
        /**                                                            \
         * The range of the text document to be manipulated. To insert \
         * text into a document create a range where start === end.    \
         */                                                            \
        Range range;                                                   \
                                                                       \
        /**                                                            \
         * The string to be inserted. For delete operations use an     \
         * empty string.                                               \
         */                                                            \
        string newText;                                                \
    }

typedef struct TextEdit
TextEdit() TextEdit;

/**
 * Additional information that describes document changes.
 *
 * @since 3.16.0
 */
typedef struct ChangeAnnotation
{
    /**
     * A human-readable string describing the actual change. The string
     * is rendered prominent in the user interface.
     */
    string label;

    /**
     * A flag which indicates that user confirmation is needed
     * before applying the change.
     */
    OPTIONAL(bool) needsConfirmation;

    /**
     * A human-readable string which is rendered less prominent in
     * the user interface.
     */
    OPTIONAL(string) description;
} ChangeAnnotation;

/**
 * An identifier referring to a change annotation managed by a workspace
 * edit.
 *
 * @since 3.16.0.
 */
typedef string ChangeAnnotationIdentifier;

/**
 * A special text edit with an additional change annotation.
 *
 * @since 3.16.0.
 */
typedef struct AnnotatedTextEdit
{
    /**
     * The actual annotation identifier.
     */
    ChangeAnnotationIdentifier annotationId;

    struct TextEdit();
} AnnotatedTextEdit;

enum EditKind
{
    EDIT_KIND_TEXT_EDIT,
    EDIT_KIND_ANNOTATED_TEXT_EDIT
};

typedef struct Edit
{
    enum EditKind kind;
    union
    {
        TextEdit textEdit;
        AnnotatedTextEdit annotatedTextEdit;
    };
} Edit;

typedef struct TextDocumentEdit
{
    /**
     * The text document to change.
     */
    OptionalVersionedTextDocumentIdentifier textDocument;

    /**
     * The edits to be applied.
     *
     * @since 3.16.0 - support for AnnotatedTextEdit. This is guarded by the
     * client capability `workspace.workspaceEdit.changeAnnotationSupport`
     */
    ARRAY(Edit) edits;
} TextDocumentEdit;

typedef struct Location
{
    DocumentUri uri;
    Range range;
} Location;

typedef struct LocationLink
{
    /**
     * Span of the origin of this link.
     *
     * Used as the underlined span for mouse interaction. Defaults to the word
     * range at the mouse position.
     */
    OPTIONAL(Range) originSelectionRange;

    /**
     * The target resource identifier of this link.
     */
    DocumentUri targetUri;

    /**
     * The full target range of this link. If the target for example is a symbol
     * then target range is the range enclosing this symbol not including
     * leading/trailing whitespace but everything else like comments. This
     * information is typically used to highlight the range in the editor.
     */
    Range targetRange;

    /**
     * The range that should be selected and revealed when this link is being
     * followed, e.g the name of a function. Must be contained by the
     * `targetRange`. See also `DocumentSymbol#range`
     */
    Range targetSelectionRange;
} LocationLink;

#define Diagnostic(T)                                                         \
    struct                                                                    \
    {                                                                         \
        /**                                                                   \
         * The range at which the message applies.                            \
         */                                                                   \
        Range range;                                                          \
                                                                              \
        /**                                                                   \
         * The diagnostic's severity. Can be omitted. If omitted it is up to  \
         * the client to interpret diagnostics as error, warning, info or     \
         * hint.                                                              \
         */                                                                   \
        OPTIONAL(DiagnosticSeverity) severity;                                \
                                                                              \
        /**                                                                   \
         * The diagnostic's code, which might appear in the user interface.   \
         */                                                                   \
        /* code ?: integer | string; */                                       \
        OPTIONAL(LSPAny) code;                                                \
                                                                              \
        /**                                                                   \
         * An optional property to describe the error code.                   \
         *                                                                    \
         * @since 3.16.0                                                      \
         */                                                                   \
        codeDescription ?: CodeDescription;                                   \
                                                                              \
        /**                                                                   \
         * A human-readable string describing the source of this              \
         * diagnostic, e.g. 'typescript' or 'super lint'.                     \
         */                                                                   \
        OPTIONAL(string) source;                                              \
                                                                              \
        /**                                                                   \
         * The diagnostic's message.                                          \
         */                                                                   \
        string message;                                                       \
                                                                              \
        /**                                                                   \
         * Additional metadata about the diagnostic.                          \
         *                                                                    \
         * @since 3.15.0                                                      \
         */                                                                   \
        OPTIONAL(ARRAY(DiagnosticTag)) tags;                                  \
                                                                              \
        /**                                                                   \
         * An array of related diagnostic information, e.g. when symbol-names \
         * within a scope collide all definitions can be marked via this      \
         * property.                                                          \
         */                                                                   \
        OPTIONAL(ARRAY(DiagnosticRelatedInformation)) relatedInformation;     \
                                                                              \
        /**                                                                   \
         * A data entry field that is preserved between a                     \
         * `textDocument/publishDiagnostics` notification and                 \
         * `textDocument/codeAction` request.                                 \
         *                                                                    \
         * @since 3.16.0                                                      \
         */                                                                   \
        OPTIONAL(T) data;                                                     \
    }

typedef enum DiagnosticSeverity
{
    /**
     * Reports an error.
     */
    Error = 1,
    /**
     * Reports a warning.
     */
    Warning = 2,
    /**
     * Reports an information.
     */
    Information = 3,
    /**
     * Reports a hint.
     */
    Hint = 4,
} DiagnosticSeverity;

typedef enum DiagnosticTag
{
    /**
     * Unused or unnecessary code.
     *
     * Clients are allowed to render diagnostics with this tag faded out
     * instead of having an error squiggle.
     */
    Unnecessary = 1,
    /**
     * Deprecated or obsolete code.
     *
     * Clients are allowed to rendered diagnostics with this tag strike through.
     */
    Deprecated = 2,
} DiagnosticTag;

typedef struct DiagnosticRelatedInformation
{
    /**
     * The location of this related diagnostic information.
     */
    Location location;

    /**
     * The message of this related diagnostic information.
     */
    string message;
} DiagnosticRelatedInformation;

/**
 * Structure to capture a description for an error code.
 *
 * @since 3.16.0
 */
typedef struct CodeDescription
{
    /**
     * An URI to open with more information about the diagnostic error.
     */
    URI href;
} CodeDescription;

typedef struct Command
{
    /**
     * Title of the command, like `save`.
     */
    string title;
    /**
     * The identifier of the actual command handler.
     */
    string command;
    /**
     * Arguments that the command handler should be
     * invoked with.
     */
    OPTIONAL(ARRAY(LSPAny)) arguments;
} Command;

/**
 * Describes the content type that a client supports in various
 * result literals like `Hover`, `ParameterInfo` or `CompletionItem`.
 *
 * Please note that `MarkupKinds` must not start with a `$`. This kinds
 * are reserved for internal usage.
 */

/**
 * Plain text is supported as a content format
 */
const string PlainText__MarkupKind = "plaintext";

/**
 * Markdown is supported as a content format
 */
const string Markdown__MarkupKind = "markdown";

typedef string MarkupKind;

/**
 * A `MarkupContent` literal represents a string value which content is
 * interpreted base on its kind flag. Currently the protocol supports
 * `plaintext` and `markdown` as markup kinds.
 *
 * If the kind is `markdown` then the value can contain fenced code blocks like
 * in GitHub issues.
 *
 * Here is an example how such a string can be constructed using
 * JavaScript / TypeScript:
 * ```typescript
 * let markdown: MarkdownContent = {
 * 	kind: MarkupKind.Markdown,
 * 	value: [
 * 		'# Header',
 * 		'Some text',
 * 		'```typescript',
 * 		'someCode();',
 * 		'```'
 * 	].join('\n')
 * };
 * ```
 *
 * *Please Note* that clients might sanitize the return markdown. A client could
 * decide to remove HTML from the markdown to avoid script execution.
 */
typedef struct MarkupContent
{
    /**
     * The type of the Markup
     */
    MarkupKind kind;

    /**
     * The content itself
     */
    string value;
} MarkupContent;

/**
 * Client capabilities specific to the used markdown parser.
 *
 * @since 3.16.0
 */
typedef struct MarkdownClientCapabilities
{
    /**
     * The name of the parser.
     */
    string parser;

    /**
     * The version of the parser.
     */
    OPTIONAL(string) version;

    /**
     * A list of HTML tags that the client allows / supports in
     * Markdown.
     *
     * @since 3.17.0
     */
    OPTIONAL(ARRAY(string)) allowedTags;
} MarkdownClientCapabilities;

/**
 * Options to create a file.
 */
typedef struct CreateFileOptions
{
    /**
     * Overwrite existing file. Overwrite wins over `ignoreIfExists`
     */
    OPTIONAL(bool) overwrite;

    /**
     * Ignore if exists.
     */
    OPTIONAL(bool) ignoreIfExists;
} CreateFileOptions;

/**
 * Create file operation
 */
typedef struct CreateFile
{
    /**
     * A create
     */
    // kind: 'create';

    /**
     * The resource to create.
     */
    DocumentUri uri;

    /**
     * Additional options
     */
    OPTIONAL(CreateFileOptions) options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * @since 3.16.0
     */
    OPTIONAL(ChangeAnnotationIdentifier) annotationId;
} CreateFile;

/**
 * Rename file options
 */
typedef struct RenameFileOptions
{
    /**
     * Overwrite target if existing. Overwrite wins over `ignoreIfExists`
     */
    OPTIONAL(bool) overwrite;

    /**
     * Ignores if target exists.
     */
    OPTIONAL(bool) ignoreIfExists;
} RenameFileOptions;

/**
 * Rename file operation
 */
typedef struct RenameFile
{
    /**
     * A rename
     */
    // kind: 'rename';

    /**
     * The old (existing) location.
     */
    DocumentUri oldUri;

    /**
     * The new location.
     */
    DocumentUri newUri;

    /**
     * Rename options.
     */
    OPTIONAL(RenameFileOptions) options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * @since 3.16.0
     */
    OPTIONAL(ChangeAnnotationIdentifier) annotationId;
} RenameFile;

/**
 * Delete file options
 */
typedef struct DeleteFileOptions
{
    /**
     * Delete the content recursively if a folder is denoted.
     */
    OPTIONAL(bool) recursive;

    /**
     * Ignore the operation if the file doesn't exist.
     */
    OPTIONAL(bool) ignoreIfExists;
} DeleteFileOptions;

/**
 * Delete file operation
 */
typedef struct DeleteFile
{
    /**
     * A delete
     */
    // kind: 'delete';

    /**
     * The file to delete.
     */
    DocumentUri uri;

    /**
     * Delete options.
     */
    OPTIONAL(DeleteFileOptions) options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * @since 3.16.0
     */
    ChangeAnnotationIdentifier annotationId;
} DeleteFile;

enum DocumentChangeKind
{
    DOCUMENT_CHANGE_KIND_TEXT_DOCUMENT_EDIT,
    DOCUMENT_CHANGE_KIND_CREATE_FILE,
    DOCUMENT_CHANGE_KIND_RENAME_FILE,
    DOCUMENT_CHANGE_KIND_DELETE_FILE
};

typedef struct DocumentChange
{
    enum DocumentChangeKind kind;
    union
    {
        TextDocumentEdit textDocumentEdit;
        CreateFile createFile;
        RenameFile renameFile;
        DeleteFile deleteFile;
    };
} DocumentChange;

typedef struct WorkspaceEdit
{
    /**
     * Holds changes to existing resources.
     */
    OPTIONAL(ARRAY(MAP(DocumentUri, ARRAY(TextEdit)))) changes;

    /**
     * Depending on the client capability
     * `workspace.workspaceEdit.resourceOperations` document changes are either
     * an array of `TextDocumentEdit`s to express changes to n different text
     * documents where each text document edit addresses a specific version of
     * a text document. Or it can contain above `TextDocumentEdit`s mixed with
     * create, rename and delete file / folder operations.
     *
     * Whether a client supports versioned document edits is expressed via
     * `workspace.workspaceEdit.documentChanges` client capability.
     *
     * If a client neither supports `documentChanges` nor
     * `workspace.workspaceEdit.resourceOperations` then only plain `TextEdit`s
     * using the `changes` property are supported.
     */
    OPTIONAL(ARRAY(DocumentChange)) documentChanges;

    /**
     * A map of change annotations that can be referenced in
     * `AnnotatedTextEdit`s or create, rename and delete file / folder
     * operations.
     *
     * Whether clients honor this property depends on the client capability
     * `workspace.changeAnnotationSupport`.
     *
     * @since 3.16.0
     */
    OPTIONAL(ARRAY(MAP(string, ChangeAnnotation))) changeAnnotations;
} WorkspaceEdit;

#endif // LILY_CORE_LSP_TYPES_H
