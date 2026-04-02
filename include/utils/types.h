#ifndef _UTILS_TYPES_H_
#define _UTILS_TYPES_H_

#if defined(__GNUC__)
#  define COMPILER_GCC
#  define GCC_VERSION \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__clang__)
#  define COMPILER_CLANG
#  define CLANG_VERSION \
    (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#  define COMPILER_ICC
#elif defined(_MSC_VER)
#  define COMPILER_MSVC
#else
#  define COMPILER_UNKNOWN
#endif

#define C89 1
#define C90 1
#define C99 199901L
#define C11 201112L
#define C17 201710L
#define C23 202311L

#if defined(__STDC_VERSION__)
#  define C_STANDARD_VERSION __STDC_VERSION__
#else
#  define C_STANDARD_VERSION 0
#endif

#if C_STANDARD_VERSION >= C23
#  define HAVE_C23 1
#elif C_STANDARD_VERSION >= C17
#  define HAVE_C17 1
#elif C_STANDARD_VERSION >= C11
#  define HAVE_C11 1
#elif C_STANDARD_VERSION >= C99
#  define HAVE_C99 1
#endif

#ifdef __cplusplus
#  define BEGIN_PLUSPLUS extern "C" {
#  define END_PLUSPLUS   }
#else
#  define BEGIN_PLUSPLUS
#  define END_PLUSPLUS
#endif // __cplusplus

#if defined(__mips__) || defined(__mips32__) || defined(__mips64__)
#  define PLATFORM_MIPS
#else
#  warning "This code is not intended for your targets"
#endif

#if defined(PLATFORM_MIPS)
#  define __MIPS_NOMIPS16 __attribute__((nomips16))
#  define __MIPS_MIPS16   __attribute__((mips16))
#else
#  define __MIPS_NOMIPS16
#  define __MIPS_MIPS16
#endif

// Принудительная упаковка структур
#ifdef COMPILER_CLANG
#  define __PACKED __attribute__((packed, aligned(1)))
#  pragma clang diagnostic ignored "-Wpacked"
#else
#  define __PACKED __attribute__((packed, aligned(1)))
#endif

// Размещение в определенной секции
#define __SECTION(name) __attribute__((section(name)))

/** @brief Оптимизация */
// Обязательно inline
#define __ALWAYS_INLINE __attribute__((always_inline)) inline
// Отключить inline
#define __NOINLINE      __attribte__((noinline))
// Функция часто используется, применить агрессивную оптимизацию
#define __HOT           __attribute__((hot))
// Функция редко используется, снизить приоритет для оптимизации
#define __COLD          __attribute__((cold))
// Все вызовы внутри функции - inline
#define __FLATTEN       __attribute__((flatten))

/** @brief Контракты выполнения */
// Функция никогда не возвращается (например, exit)
#define __NORETURN      __attribute__((noreturn))
// Зависит только только аргументов + глобальных переменны
#define __PURE          __attribute__((pure))
// Зависит только от аргументов
#define __CONST         __attribute__((const))

#define __FORMAT(archetype, string_index, first_to_check) \
  __attribute__((format(archetype, string_index, first_to_check)))

/** @brief Атрибуты инициализации и окочания */
// Вызывается перед main()
#define __CONSTRUCTOR              __attribute__((constructor))
// Приоритетный вызов перед main(). Меньшие значения вызываются раньше
#define __CONSTRUCTOR_PR(priority) __attribute__((constructor(priority)))
// Вызывается после main()
#define __DESTRUCTOR               __attribute__((destructor))

// Вызывает предупреждение, если результат фунукции не используется
#define __WARN_UNUSED_RESULT       __attribute__((warn_unused_result))

/** @brief Атбриты оптимизации */
#define __NO_OPTIMIZE              __attribute__((optimize("O0")))
#define __OPTIMIZE_SPEED           __attribute__((optimize("O2")))
#define __OPTIMIZE_SIZE            __attribute__((optimize("Os")))

// Флаги не использования/использования.
#define __UNUSED                   __attribute__((unused))
#define __USED                     __attribute__((used))

/** @brief Атрибуты линковки и видимости  */
// Символ экспорта (по умолчанию)
#define __VISIBLE                  __attribute__((visibility("default")))
// Скрыть символ, не экспортируется
#define __HIDDEN                   __attribute__((visibility("hidden")))
// Создать ссылку (alias) на фукцию
#define __ALIAS(symbol)            __attribute__((weak, alias(#symbol)))
// Слабый символ, может быть переопределен
#define __WEAK                     __attribute__((weak))

// Метка для обработчика прерывания
#define __INTERRUPT                __attribute__((interrupt))
// Убирает пролог/эпилог для функции (в коде используется только голый asm код)
#define __NAKED                    __attribute__((naked))

#define __SHIDDEN                  __HIDDEN static

// Заглушка для реализации функции
#define TODO_FUNCTION()                                         \
  do {                                                          \
    _Pragma("warning \"TODO: A function is not implemented\""); \
    __builtin_trap();                                           \
  } while (0)

// NOLINTBEGIN(readability-identifier-naming)
// Типы с явным указанием размера
typedef signed char        i8;
typedef unsigned char      u8;
typedef signed short       i16;
typedef unsigned short     u16;
typedef signed int         i32;
typedef unsigned int       u32;
typedef signed long long   i64;
typedef unsigned long long u64;

// Типы для архитектурно-зависимых размеров
typedef i32 intptr;
typedef u32 uintptr;

// Типы для размеров
typedef u32 usize;
typedef i32 ssize;
typedef u64 usize64;
typedef i64 ssize64;

// Типы для адресов памяти (MIPS32)
typedef u32 phys_addr_t; // Физический адрес
typedef u32 virt_addr_t; // Виртуальный адрес

typedef void *vptr;

#ifndef NULL
#  define NULL ((void *) 0)
#endif // NULL

#ifndef offsetof
#  define offsetof(type, member) ((usize) & (((type *) 0)->member))
#endif // offsetof

// Boolean тип
#ifndef __cplusplus
typedef _Bool boolean;
#  define true  1
#  define false 0
#else
typedef bool boolean;
#endif // cplusplus

// Указатели на функции
typedef void (*void_func_t)(void);

// NOLINTEND(readability-identifier-naming)

#define LINKER_VAR(name)     extern u8 name[]

// Макросы для работы с битами
#define BIT(n)               (1U << (n))
#define BIT_MASK(n)          (BIT(n) - 1U)
#define SET_BIT(reg, bit)    ((reg) |= BIT(bit))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~BIT(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= BIT(bit))
// #define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1U)
#define READ_BIT(reg, bit)   ((reg) & BIT(bit))
#define CHECK_BIT(byte, bit) READ_BIT(byte, bit)
#define WRITE_BIT(reg, bit, value) \
  ((reg) = ((reg) & ~BIT(bit)) | ((value) ? BIT(bit) : 0U))

#define SET_BITS(byte, mask)            ((byte) |= (mask))
#define CLEAR_BITS(byte, mask)          ((byte) &= ~(mask))
#define ALL_BITS_SET(byte, mask)        (((byte) & (mask)) == (mask))
#define ANY_BIT_SET(byte, mask)         (((byte) & (mask)) != 0)
#define EXTRACT_BITS(byte, mask, shift) (((byte) & (mask)) >> (shift))
#define INSERT_BITS(byte, value, mask, shift) \
  ((byte) = ((byte) & ~(mask)) | (((value) << (shift)) & (mask)))

// Циклический сдвиг влево (rotate left)
#define ROL(value, shift) \
  (((value) << (shift)) | ((value) >> (sizeof(value) * 8 - (shift))))
// Циклический сдвиг вправо (rotate right)
#define ROR(value, shift) \
  (((value) >> (shift)) | ((value) << (sizeof(value) * 8 - (shift))))

#define BIT_RANGE(high, low)   (BIT_MASK((high) - (low) + 1) << (low))

// Макросы для выравнивания
#define ALIGN_UP(val, align)   (((val) + (align) - 1) & ~((align) - 1))
#define ALIGN_DOWN(val, align) ((val) & ~((align) - 1))
#define IS_ALIGNED(ptr, align) (((uintptr) (ptr) & ((align) - 1)) == 0)

// Выравнивание данных для MIPS
#define ALIGNED(n)             __attribute__((aligned(n)))

#if defined(PLATFORM_MIPS)
#  define MIPS_CACHE_ALIGNED ALIGNED(32)
#  define MIPS_WORD_ALIGNED  ALIGNED(4)
#  define MIPS_ALIGNMENT     4
#else
#  define MIPS_CACHE_ALIGNED
#  define MIPS_WORD_ALIGNED
#  define MIPS_ALIGNMENT
#endif

// Макросы для минимального/максимального
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(val, min, max) \
  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

// Макросы для подсчета элементов массива
#define ARRAY_SIZE(arr)                   (sizeof(arr) / sizeof((arr)[0]))

// Макросы для доступа к памяти с барьерами
#define MEM_READ8(addr)                   (*(volatile u8 *) (addr))
#define MEM_WRITE8(addr, val)             *(volatile u8 *) (addr) = (val)
#define MEM_SET8(addr, val)               *(volatile u8 *) (addr) |= (val)

#define MEM_READ16(addr)                  (*(volatile u16 *) (addr))
#define MEM_WRITE16(addr, val)            *(volatile u16 *) (addr) = (val)
#define MEM_SET16(addr, val)              *(volatile u16 *) (addr) |= (val)

#define MEM_READ32(addr)                  (*(volatile u32 *) (addr))
#define MEM_WRITE32(addr, val)            *(volatile u32 *) (addr) = (val)
#define MEM_SET32(addr, val)              *(volatile u32 *) (addr) |= (val)

#define IS_LITTLE_ENDIAN                  1
#define IS_BIG_ENDIAN                     0

// NOLINTBEGIN(readability-identifier-naming, bugprone-reserved-identifier)
// Секции памяти для ELF
#define __CODE_SECTION                    __SECTION(".text")
#define __DATA_SECTION                    __SECTION(".data")
#define __BSS_SECTION                     __SECTION(".bss")
#define __RODATA_SECTION                  __SECTION(".rodata")
#define __SMALL_DATA_SECTION              __SECTION(".sdata")
#define __SMALL_BSS_SECTION               __SECTION(".sbss")

// Макросы для volatile доступа
#define VOLATILE_READ(addr, type)         (*(volatile(type) *) (addr))
#define VOLATILE_WRITE(addr, type, value) *(volatile(type) *) (addr) = (value)
#define VOLATILE_SET(addr, type, value)   *(volatile(type) *) (addr) |= (value)

#define NOP()                             __asm__("nop")
#define NOPS(n)                                       \
  {                                                   \
    for (u32 i_ = 0; i_ < (u32) (n); ++i_) { NOP(); } \
  }

// Байт-своп для 16-битных значений
#define BUILTIN_BSWAP16(x) ((((x) >> 8) & 0x00FF) | (((x) << 8) & 0xFF00))

// Байт-своп для 32-битных значений
#define BUILTIN_BSWAP32(x)                                \
  ((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >> 8) \
   | (((x) & 0x0000FF00) << 8) | (((x) & 0x000000FF) << 24))

// Выравнивание указателя
#define BUILTIN_ALIGN(p, align) \
  ((typeof(p)) (((uintptr) (p) + (align) - 1) & ~((align) - 1)))

// Оптимизационные подсказки
#if __has_builtin(__builtin_expect)
#  define __LIKELY(x)   __builtin_expect(!!(x), 1)
#  define __UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define __LIKELY(x)   (x)
#  define __UNLIKELY(x) (x)
#endif // __builtin_expect

#if __has_builtin(__builtin_prefetch)
/**
 * @brief __builtin_prefetch - указывает процессору о необходимости
 * заранее загрузить данные в хеш.
 *
 * __buildit_prefetch(const void* addr, int rw = 0, int locality = 3)
 * - rw - тип доступа (0 - чтение, 1 - запись)
 * - locality - уровень локальности (0-3):
 * - - 0 - нет локальности (данные используются один раз)
 * - - 1 - низкая локальность
 * - - 2 - средняя локальность
 * - - 3 - высокая временная локальность (данные будут использоваться много раз)
 */
#  if defined(PLATFORM_MIPS)
#    define __MIPS_PREFETCH(addr)   __builtin_prefetch(addr, 0, 3)
#    define __MIPS_PREFETCH_W(addr) __builtin_prefetch(addr, 1, 3)
#  else
#    define __MIPS_PREFETCH(addr)
#    define __MIPS_PREFETCH_W(addr)
#  endif
#else
#  warning "Not implemented __MIPS_PREFETCH & __MIPS_PREFETCH_W"
#  define __MIPS_PREFETCH(addr)
#  define __MIPS_PREFETCH_W(addr)
#endif

// Метки для аппаратных регистров
#define __I  volatile // Разрешено устанавливать биты регистра
#define __IO volatile // Разрешено устанавливать и считывать биты регистра
#define __O  volatile // Разрешено считывать биты регистра
// NOLINTEND(readability-identifier-naming, bugprone-reserved-identifier)

// Статус операции
typedef enum {
  STATUS_OK = 0,
  STATUS_ERROR,
  STATUS_TIMEOUT,
  STATUS_INVALID_PARAM,
  STATUS_NOT_SUPPORTED,
  STATUS_BUSY,
} status_t;

// Максимальные размеры типов данных
#define UINT8_MAX  ((u8) 0xFF)
#define UINT16_MAX ((u16) 0xFFFF)
#define UINT32_MAX ((u32) 0xFFFFFFFF)
#define UINT64_MAX ((u64) 0xFFFFFFFFFFFFFFFF)

#define INT8_MAX   ((i8) 127)
#define INT8_MIN   ((i8) (-INT8_MAX - 1))
#define INT16_MAX  ((i16) 32767)
#define INT16_MIN  ((i16) (-INT16_MAX - 1))
#define INT32_MAX  ((i32) 2147483647)
#define INT32_MIN  ((i32) (-INT32_MAX - 1))
#define INT64_MAX  ((i64) 9223372036854775807LL)
#define INT64_MIN  ((i64) (-INT64_MAX - 1))

#define USIZE_MAX  UINT32_MAX
#define SSIZE_MAX  INT32_MAX
#define SSIZE_MIN  INT32_MIN
#endif // _UTILS_TYPES_H_
