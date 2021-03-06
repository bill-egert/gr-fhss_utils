/* -*- c++ -*- */
/*
 * Copyright 2018, 2019, 2020 National Technology & Engineering Solutions of Sandia, LLC
 * (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government
 * retains certain rights in this software.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_FHSS_UTILS_CF_ESTIMATE_IMPL_H
#define INCLUDED_FHSS_UTILS_CF_ESTIMATE_IMPL_H

#include <gnuradio/blocks/rotator.h>
#include <gnuradio/fft/fft.h>
#include <fhss_utils/cf_estimate.h>

namespace gr {
namespace fhss_utils {

class cf_estimate_impl : public cf_estimate
{
private:
    // message handlers
    void pdu_handler(pmt::pmt_t pdu);

    // this block supports a few different estimation methods
    int d_method;

    // fft tools
    void fft_setup(int power);
    void fft_cleanup();
    std::vector<gr::fft::fft_complex*> d_ffts;
    std::vector<float*> d_windows;
    float* d_mags2;
    float* d_magnitude_shifted_f;
    const float d_gauss_sigma;

    // correction tools
    blocks::rotator d_rotate;
    std::vector<gr_complex> d_corrected_burst;

    // coerce tools
    std::vector<float> d_channel_freqs;
    float coerce_frequency(float center_frequency, float sample_rate);
    float rms(std::vector<float> mags2,
              std::vector<float> freq_axis,
              float center_frequency,
              float sample_rate);
    float half_power(std::vector<float> mags2);

    // BW and SNR estimation
    float rms_bw(std::vector<float> mags2,
                 std::vector<float> freq_axis,
                 float center_frequency);
    float snr_estimation(std::vector<float> mags2,
                         std::vector<float> freq_axis,
                         float center_frequency,
                         float bandwidth,
                         float sample_rate);

    // intern'ed pmts
    pmt::pmt_t PMT_CENTER_FREQUENCY = pmt::intern("center_frequency");
    pmt::pmt_t PMT_RELATIVE_FREQUENCY = pmt::intern("relative_frequency");
    pmt::pmt_t PMT_SAMPLE_RATE = pmt::intern("sample_rate");
    pmt::pmt_t PMT_BANDWIDTH = pmt::intern("bandwidth");
    pmt::pmt_t PMT_SNRDB = pmt::intern("snr_db");
    pmt::pmt_t PMT_IN = pmt::intern("in");
    pmt::pmt_t PMT_OUT = pmt::intern("out");
    pmt::pmt_t PMT_DEBUG = pmt::intern("debug");

public:
    /**
     * Constructor
     *
     * @param method - estimate method #cf_method
     * @param channel_freqs - channel freq list for coerce method
     */
    cf_estimate_impl(int method, std::vector<float> channel_freqs);

    /**
     * Deconstructor
     */
    ~cf_estimate_impl();

    // getters and setters

    /**
     * set coerce channel frequency list
     *
     * @param channel_freqs - channel freq list for coerce method
     */
    void set_freqs(std::vector<float> channel_freqs);

    /**
     * Set estimate method
     *
     * @param method - estimate method #cf_method
     */
    void set_method(int method);
};

} // namespace fhss_utils
} // namespace gr

#endif /* INCLUDED_FHSS_UTILS_CF_ESTIMATE_IMPL_H */
