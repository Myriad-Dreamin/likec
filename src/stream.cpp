



#ifndef STREAM_TTTT_CPP
#define STREAM_TTTT_CPP
#include <queue>
#include <string>
#include <sstream>

namespace parse {

template<typename stream_t, int64_t buffer_size>
class Stream {
    typedef bool even_check[-(buffer_size & 1)];
    typedef bool zero_check[buffer_size - 1];

    const size_t stream_unit_size = sizeof(stream_t);
    const size_t half_buffer_size = buffer_size >> 1;

    int64_t last_column, lastlast_column;
    int64_t recorded_row, recorded_col, recorded_offset;
    int64_t row, col;
    std::istream *in;
    std::deque<stream_t> buffer;
    std::deque<stream_t> unread_buffer;
    
    public:
    Stream(std::istream *in) {
        this->in = in; (*in)  >> std::noskipws;
        this->recorded_offset = 0;
        last_column = lastlast_column = 0;
        recorded_row = 1; recorded_col = 1;
        row = 1; col = 0;
        this->in->width(stream_unit_size);
    };
    Stream(const Stream &s) { throw std::exception("do not copy stream"); };
    Stream(const Stream &&s) {
        this->buffer = std::move(s.buffer);
        this->unread_buffer = std::move(s.unread_buffer);
        
        this->in = in;
        this->lastlast_column = s.lastlast_column;
        this->last_column = s.last_column;
        this->recorded_row = s.recorded_row; this->recorded_col = s.recorded_col;
        this->row = s.row; this->col = s.col;
        s.in = nullptr;
    };

    stream_t Read() {
        fit(buffer);
        return last(buffer);
    }

    stream_t Unread() {
        if (unread_buffer.size() == 0) {
            throw std::runtime_error("unread too many size");
        }
        stream_t bit = unread_buffer.back();
        buffer.push_front(bit);
        unread_buffer.pop_back();
        col--; recorded_offset--;
        if (bit == '\n') {
            col = last_column;
            last_column = lastlast_column;
            row--;
        }
        fit(buffer);
        return bit;
    }

    void get_row_col(int64_t &row, int64_t &col) {
        row = this->recorded_row;
        col = this->recorded_col;
    }

    void collect_lines(std::basic_string<stream_t> &line, std::basic_string<stream_t> &line_pointer) {
        stream_t bit;
        while(unread_buffer.size() && ((bit = Unread()) != '\n'));
        if (unread_buffer.size() == 0 && bit != '\n') {
            line = "<line is too long>";
            line_pointer = "";
            return;
        }
        bit = Read();
        int64_t c = 1;
        while((bit = Read()) != EOF) {
            if (bit == '\n') {
                break;
            }
            if (isprint(bit)) {
                line.push_back(bit);
                line_pointer.push_back(c == this->recorded_col ? '^' : ' ');
            } else {
                line.push_back('['); 
                line_pointer.push_back(' ');
                line_pointer.push_back(c == this->recorded_col ? '^' : ' ');
                std::stringstream sstream;
                sstream << std::hex << bit;
                std::basic_string<stream_t> num = sstream.str();
                for(int i = (stream_unit_size << 1) - num.length(); i > 0; i--) {
                    line.push_back('0');
                }
                line += num;
                for(int i = (stream_unit_size << 1); i > 0; i--) {
                    line_pointer.push_back(' ');
                }
                line.push_back(']'); 
                line_pointer.push_back(' ');
            }
            c++;
        }        
        this->row = this->recorded_row + 1;
        this->col = 0;
        // if (in->eof()) {
        //     in->clear();
        //     in->seekg(0, std::ios::end);
        //     std::cout << "eof" << in->tellg() << " " << this->recorded_offset << " ";
        // }
        // if (in->tellg() == -1) {
        //     throw std::runtime_error("error on unrevertable input stream");
        // }
        // std::cout << "rc" << this->recorded_row << " " << this->recorded_col << " ";
        // std::cout << -((this->recorded_col - 1) * int64_t(stream_unit_size) + (static_cast<int64_t>(in->tellg()) - this->recorded_offset))<< std::endl;
        
        // in->seekg(1 + this->recorded_offset - (this->recorded_col - 1) * stream_unit_size, std::ios::beg);
        // std::cout << "eof" << static_cast<int64_t>(in->tellg()) << " " << this->recorded_offset << " ";
        // stream_t bit;
        // int64_t c = 0;
        // while ((*in) >> bit) {
            // if (bit == '\n') {
            //     break;
            // }
            // if (isprint(bit)) {
            //     line.push_back(bit);
            //     line_pointer.push_back(c == this->recorded_col ? '^' : ' ');
            // } else {
            //     line.push_back('['); 
            //     line_pointer.push_back(' ');
            //     line_pointer.push_back(c == this->recorded_col ? '^' : ' ');
            //     std::stringstream sstream;
            //     sstream << std::hex << bit;
            //     std::basic_string<stream_t> num = sstream.str();
            //     for(int i = (stream_unit_size << 1) - num.length(); i > 0; i--) {
            //         line.push_back('0');
            //     }
            //     line += num;
            //     for(int i = (stream_unit_size << 1); i > 0; i--) {
            //         line_pointer.push_back(' ');
            //     }
            //     line.push_back(']'); 
            //     line_pointer.push_back(' ');
            // }
            // c++;
        // }
        // this->row = this->recorded_row + 1;
        // this->col = 0;
    }

    void record() {
        this->recorded_row = this->row;
        this->recorded_col = this->col;
    }

    private:
    inline void shrink(std::deque<stream_t> &buffer) {
        if (buffer.size() > buffer_size) {
            if (in->fail()) {
                in->seekg(0, std::ios::end);
            }
            if (in->tellg() == -1) {
                throw std::runtime_error("error on unseekable input stream");
            }
            in->seekg((buffer.size() - buffer_size) * stream_unit_size, std::ios::cur);

            int64_t sh_size = static_cast<int64_t>(buffer.size()) - 
                buffer_size + static_cast<int64_t>(unread_buffer.size()) - buffer_size;
            
            if (sh_size > unread_buffer.size()) { sh_size = unread_buffer.size(); }
            while( sh_size-- ) { unread_buffer.pop_front(); }

            sh_size = (buffer_size << 1) - unread_buffer.size();
            while (buffer.size() > sh_size) {
                buffer.pop_front();
            }
            while (buffer.size() > buffer_size) {
                unread_buffer.push_back(buffer.front());
                buffer.pop_front();
            }
        }
    }

    inline void fill(std::deque<stream_t> &buffer) {
        stream_t bit;
        while (buffer.size() < half_buffer_size && ((*in) >> bit)) {
            // std::cout << "[ "<< bit <<" ]";
            buffer.push_back(bit);
        }
    }

    inline void fit(std::deque<stream_t> &buffer) {
        if (buffer.size() > buffer_size) {
            shrink(buffer);
        } else if (buffer.size() == 0) {
            fill(buffer);
        }
    }

    inline stream_t last(std::deque<stream_t> &buffer) {
        stream_t bit;
        if (buffer.size() == 0) {
            bit = EOF;
        } else {
            bit = buffer.front(); buffer.pop_front();
        }
        
        col++; recorded_offset++;
        if (bit == '\n') {
            last_column = col;
            lastlast_column = last_column;
            col = 0;
            row++;
        }
        unread_buffer.push_back(bit);
        if(unread_buffer.size() > buffer_size) {
            unread_buffer.pop_front();
        }
        return bit;
    }

};
}

#endif