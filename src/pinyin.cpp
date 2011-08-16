#include "pinyin.h"

/* Place holder for combining static libraries in sub-directories. */

/* Note: will implement a glue layer for input method integration.
 * Refer to: fcitx-sunpinyin
 */

struct _pinyin_context_t{
    PinyinDefaultParser * m_default_parser;
    PinyinShuangPinParser * m_shuang_pin_parser;
    PinyinCustomSettings * m_custom;

    PinyinLargeTable * m_pinyin_table;
    PhraseLargeTable * m_phrase_table;
    FacadePhraseIndex * m_phrase_index;
    Bigram * m_system_bigram;
    Bigram * m_user_bigram;

    PinyinLookup * m_pinyin_lookup;
    PhraseLookup * m_phrase_lookup;
    PinyinKeyVector m_pinyin_keys;
    MatchResults m_match_results;
    CandidateConstraints m_constraints;

    BitmapPinyinValidator * m_validator;
    const char * m_system_dir;
    const char * m_user_dir;
};

pinyin_context_t * pinyin_init(const char * systemdir, const char * userdir);
void pinyin_fini(pinyin_context_t * context);

/* copy from custom to context->m_custom. */
bool pinyin_set_options(pinyin_context_t * context,
                        PinyinCustomSettings * custom);
/* copy from pinyin_keys to m_pinyins. */
bool pinyin_set_pinyin_keys(pinyin_context_t * context,
                            PinyinKeyVector pinyin_keys);


/* the returned sentence should be freed by g_free(). */
bool pinyin_get_guessed_sentence(pinyin_context_t * context,
                                char ** sentence);

bool pinyin_parse_full(pinyin_context_t * context,
                       const char * onepinyin,
                       PinyinKey * onekey);
bool pinyin_parse_more_fulls(pinyin_context_t * context,
                             const char * pinyins,
                             PinyinKeyVector pinyin_keys);

bool pinyin_parse_double(pinyin_context_t * context,
                         const char * onepinyin,
                         PinyinKey * onekey);
bool pinyin_parse_more_doubles(pinyin_context_t * context,
                               const char * pinyins,
                               PinyinKeyVector pinyin_keys);

bool pinyin_get_candidates(pinyin_context_t * context,
                           size_t offset, TokenVector tokens);
bool pinyin_choose_candidate(pinyin_context_t * context,
                             size_t offset, phrase_token_t token);
bool pinyin_clear_constraints(pinyin_context_t * context);

/* the returned word should be freed by g_free. */
bool pinyin_translate_token(pinyin_context_t * context,
                            phrase_token_t token, char ** word){
    PhraseItem item;
    utf16_t buffer[MAX_PHRASE_LENGTH];

    bool retval = context->m_phrase_index->get_phrase_item(token, item);
    m_cache_phrase_item.get_phrase_string(buffer);
    guint8 length = m_cache_phrase_item.get_phrase_length();
    *word = g_utf16_to_utf8(buffer, length, NULL, NULL, NULL);
    return retval;
}

bool pinyin_train(pinyin_context_t * context){
    bool retval = context->m_pinyin_lookup->train_result
        (context->m_piinyin_keys, context->m_constraints,
         context->m_match_results);
    return retval;
}

bool pinyin_save(pinyin_context_t * context);

bool pinyin_reset(pinyin_context_t * context);

/** TODO: to be implemented.
 *  bool pinyin_get_guessed_sentence_with_prefix(...);
 *  bool pinyin_get_candidates_with_prefix(...);
 *  For context-dependent order of the candidates list.
 */
