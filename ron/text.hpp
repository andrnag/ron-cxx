#ifndef ron_open_text_hpp
#define ron_open_text_hpp
#include "op.hpp"

namespace ron {

class TextFrame {
    const std::string data_;

   public:
    typedef std::vector<TextFrame> Batch;

    TextFrame() : data_{} {}
    explicit TextFrame(const std::string& data) : data_{data} {}
    explicit TextFrame(const std::string&& data) : data_{data} {}

    const std::string& data() const { return data_; }

    class Cursor {
        // the cursor does not own the memory
        slice_t data_;
        Op op_;
        int pos_;
        int off_;
        int cs;
        Uuid prev_id_;

        static constexpr int RON_FULL_STOP = 255;
        static constexpr int SPEC_SIZE = 2;  // open RON

       public:
        explicit Cursor(const slice_t& data)
            : data_{data},
              op_{TERM::RAW},
              cs{0},
              off_{0},
              pos_{-1},
              prev_id_{} {
            Next();
        }
        explicit Cursor(const TextFrame& host) : Cursor{host.data_} {}
        const Op& op() const { return op_; }
        bool Next();
        inline bool valid() const { return cs != 0; }
        const slice_t data() const { return data_; }
        inline slice_t slice(frange_t range) const {
            return data().slice(range);
        }
        inline const Uuid& id() const { return op_.id(); }
        inline const Uuid& ref() const { return op_.ref(); }
        int64_t parse_int(fsize_t idx);
        double parse_float(fsize_t idx);
        void parse(fsize_t idx) {
            switch (op().type(idx)) {
                case INT:
                    parse_int(idx);
                    break;
                case FLOAT:
                    parse_float(idx);
                    break;
                default:
                    break;
            }
        }
        std::string unescape(const slice_t& data) const;
        inline std::string parse_string(fsize_t idx) const {
            const Atom& atom = op().atom(idx);
            assert(atom.type() == STRING);
            return unescape(slice(atom.origin().range()));
        }
        inline Uuid parse_uuid(fsize_t idx) {
            assert(op_.atom(idx).type() == UUID);
            return op_.uuid(idx);
        }
    };

    struct Builder {
        TERM term_;
        Uuid prev_;
        std::string data_;

        inline void Write(char c) { data_.push_back(c); }
        inline void Write(slice_t data) { data_.append(data.buf_, data.size_); }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(const Uuid& value);
        void WriteString(const std::string& value);

        void escape(std::string& escaped, const slice_t& unescaped);

       public:
        Builder() : term_{RAW}, prev_{Uuid::ZERO}, data_{} {}

        // copy-as-strings
        void AppendOp(const Cursor& cur);

        // RON coding conversion (parsing, re-serialization)
        template <typename Cursor2>
        void AppendOp(const Cursor2& cur);

        const TextFrame frame() const { return TextFrame{data_}; }

        bool empty() const { return data_.empty(); }

        //  B E A U T I F Y I N G   T E M P L A T E S

        // terminates the op
        void AppendAtoms() { Write(TERM_PUNCT[term_]); }

        template <typename... Ts>
        void AppendAtoms(int64_t value, Ts... args) {
            Write(' ');
            WriteInt(value);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendAtoms(Uuid value, Ts... args) {
            Write(value.is_ambiguous() ? ' ' : ATOM_PUNCT[UUID]);
            WriteUuid(value);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendAtoms(double value, Ts... args) {
            Write(' ');
            WriteFloat(value);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendAtoms(const std::string& value, Ts... args) {
            Write(ATOM_PUNCT[STRING]);
            WriteString(value);
            Write(ATOM_PUNCT[STRING]);
            AppendAtoms(args...);
        }

        void AppendSpec(const Uuid& id, const Uuid& ref) {
            bool seq_id = id == prev_.inc();
            if (!seq_id) {
                Write(SPEC_PUNCT[EVENT]);
                WriteUuid(id);
            }
            if (ref != prev_) {
                if (!seq_id) Write(' ');
                Write(SPEC_PUNCT[REF]);
                WriteUuid(ref);
            }
            prev_ = id;
        }

        template <typename... Ts>
        void AppendNewOp(TERM term, const Uuid& id, const Uuid& ref,
                         Ts... args) {
            term_ = term;
            AppendSpec(id, ref);
            AppendAtoms(args...);
        }

        template <typename Cur>
        void AppendAll(Cur& cur) {
            if (!cur.valid()) return;
            do {
                AppendOp(cur);
            } while (cur.Next());
        }

        template <typename Frame2>
        void AppendFrame(const Frame2& frame) {
            auto cur = frame.cursor();
            AppendAll(cur);
        }
    };

    Cursor cursor() const { return Cursor{*this}; }

    static constexpr char ESC = '\\';

    typedef std::vector<Cursor> Cursors;
};

}  // namespace ron

namespace std {

inline void swap(ron::TextFrame::Builder& builder, std::string& str) {
    swap(builder.data_, str);
}

}  // namespace std

#endif
