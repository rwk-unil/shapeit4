/*******************************************************************************
 * Copyright (C) 2021 Rick Wertenbroek, University of Lausanne
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
 ******************************************************************************/
#ifndef __ACCESSOR_HPP__
#define __ACCESSOR_HPP__

#include "accessor_internals.hpp"

class Accessor {
public:

    Accessor(std::string& filename);
    virtual ~Accessor();

    void fill_genotype_array(int32_t* gt_arr, size_t gt_arr_size, size_t n_alleles, size_t position) {
        internals->fill_genotype_array(gt_arr, gt_arr_size, n_alleles, position);
    }

    int get_genotypes(const bcf_hdr_t *hdr, bcf1_t *line, void **gt_arr, int *gt_arr_size) {
        size_t ngt = header.hap_samples; /// @todo ploidy

		if (!*gt_arr) *gt_arr = malloc(sizeof(int)*ngt);
        *gt_arr_size = ngt;

		// extraction is done by having the accessor seek the data at "BM"
		int count = 0;
		int ret = bcf_unpack(line, BCF_UN_ALL);
		if (ret) { std::cerr << "bcf_unpack error" << std::endl; }
		if (bcf_get_format_int32(hdr, line, "BM", &values, &count) < 1) {
			std::cerr << "Failed to retrieve binary matrix index position (BM key)" << std::endl;
			throw "BM key value not found";
		}

        fill_genotype_array((int32_t*)*gt_arr, ngt, line->n_allele, values[0]);

        return ngt;
    }

    /// @todo All these dependencies on the filenames are dirty and should be fixed ...
    std::string get_variant_filename() {
        std::stringstream ss;
		ss << filename << "_var.bcf";
		return ss.str();
    }

    static std::string get_variant_filename(const std::string& fname) {
        std::stringstream ss;
		ss << fname << "_var.bcf";
		return ss.str();
    }

    static std::string get_filename_from_variant_file(const std::string& fname) {
        std::string filename(fname);
        auto pos = filename.find("_var.bcf");
        if (pos != std::string::npos) {
            filename.erase(pos, filename.length());
        } else {
            //std::cerr << "Cannot convert filename " << filename << std::endl;
            throw "Cannot convert filename";
        }

        return filename;
    }

    std::vector<std::string>& get_sample_list() {return sample_list;}
    size_t get_number_of_samples() const {return sample_list.size();}
    const header_t& get_header_ref() const {return header;}

protected:
    std::unique_ptr<AccessorInternals> internals;
    std::string filename;
    header_t header;
    std::vector<std::string> sample_list;
    int *values{NULL};
};

#endif /* __ACCESSOR_HPP__ */