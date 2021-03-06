#include "CPUCNNProcessor.hpp"

#define RULE(x) std::max(x, 0.0)
#define NORMB(X) (static_cast<double>(X) / 255.0)
#define NORMW(X) (static_cast<double>(X) / 65535.0)
#define UNNORMB(n) ((n) >= 1.0? (uint8_t)(255) : ((n) <= 0.0 ? (uint8_t)(0) : static_cast<uint8_t>(n * 255.0 + 0.5)))
#define UNNORMW(n) ((n) >= 1.0? (uint16_t)(65535) : ((n) <= 0.0 ? (uint16_t)(0) : static_cast<uint16_t>(n * 65535.0 + 0.5)))
#define CLAMP(v, lo, hi) ((v < lo) ? lo : (hi < v) ? hi : v)

void Anime4KCPP::CPU::CNNProcessor::conv1To8B(const cv::Mat& img, const double* kernels, const double* biases, cv::Mat& tmpMat)
{
    const int channels = 8;
    const int srcChannels = img.channels();
    const int lineStep = img.cols * srcChannels;
    changEachPixel1ToN(img, [&](const int i, const int j, ChanD outMat, LineB curLine) {
        const int orgJ = j / channels * srcChannels;
        const int jp = orgJ < (img.cols - 1)* srcChannels ? srcChannels : 0;
        const int jn = orgJ > srcChannels ? -srcChannels : 0;
        const LineB pLineData = i < img.rows - 1 ? curLine + lineStep : curLine;
        const LineB cLineData = curLine;
        const LineB nLineData = i > 0 ? curLine - lineStep : curLine;

        const PixelB tl = nLineData + orgJ + jn, tc = nLineData + orgJ, tr = nLineData + orgJ + jp;
        const PixelB ml = cLineData + orgJ + jn, mc = cLineData + orgJ, mr = cLineData + orgJ + jp;
        const PixelB bl = pLineData + orgJ + jn, bc = pLineData + orgJ, br = pLineData + orgJ + jp;

        const double tln = NORMB(tl[Y]);
        const double tcn = NORMB(tc[Y]);
        const double trn = NORMB(tr[Y]);
        const double mln = NORMB(ml[Y]);
        const double mcn = NORMB(mc[Y]);
        const double mrn = NORMB(mr[Y]);
        const double bln = NORMB(bl[Y]);
        const double bcn = NORMB(bc[Y]);
        const double brn = NORMB(br[Y]);

        outMat[0] =
            RULE(
                tln * kernels[0 * 9 + 0] + tcn * kernels[0 * 9 + 1] + trn * kernels[0 * 9 + 2] +
                mln * kernels[0 * 9 + 3] + mcn * kernels[0 * 9 + 4] + mrn * kernels[0 * 9 + 5] +
                bln * kernels[0 * 9 + 6] + bcn * kernels[0 * 9 + 7] + brn * kernels[0 * 9 + 8] + biases[0]);
        outMat[1] =
            RULE(
                tln * kernels[1 * 9 + 0] + tcn * kernels[1 * 9 + 1] + trn * kernels[1 * 9 + 2] +
                mln * kernels[1 * 9 + 3] + mcn * kernels[1 * 9 + 4] + mrn * kernels[1 * 9 + 5] +
                bln * kernels[1 * 9 + 6] + bcn * kernels[1 * 9 + 7] + brn * kernels[1 * 9 + 8] + biases[1]);
        outMat[2] =
            RULE(
                tln * kernels[2 * 9 + 0] + tcn * kernels[2 * 9 + 1] + trn * kernels[2 * 9 + 2] +
                mln * kernels[2 * 9 + 3] + mcn * kernels[2 * 9 + 4] + mrn * kernels[2 * 9 + 5] +
                bln * kernels[2 * 9 + 6] + bcn * kernels[2 * 9 + 7] + brn * kernels[2 * 9 + 8] + biases[2]);
        outMat[3] =
            RULE(
                tln * kernels[3 * 9 + 0] + tcn * kernels[3 * 9 + 1] + trn * kernels[3 * 9 + 2] +
                mln * kernels[3 * 9 + 3] + mcn * kernels[3 * 9 + 4] + mrn * kernels[3 * 9 + 5] +
                bln * kernels[3 * 9 + 6] + bcn * kernels[3 * 9 + 7] + brn * kernels[3 * 9 + 8] + biases[3]);
        outMat[4] =
            RULE(
                tln * kernels[4 * 9 + 0] + tcn * kernels[4 * 9 + 1] + trn * kernels[4 * 9 + 2] +
                mln * kernels[4 * 9 + 3] + mcn * kernels[4 * 9 + 4] + mrn * kernels[4 * 9 + 5] +
                bln * kernels[4 * 9 + 6] + bcn * kernels[4 * 9 + 7] + brn * kernels[4 * 9 + 8] + biases[4]);
        outMat[5] =
            RULE(
                tln * kernels[5 * 9 + 0] + tcn * kernels[5 * 9 + 1] + trn * kernels[5 * 9 + 2] +
                mln * kernels[5 * 9 + 3] + mcn * kernels[5 * 9 + 4] + mrn * kernels[5 * 9 + 5] +
                bln * kernels[5 * 9 + 6] + bcn * kernels[5 * 9 + 7] + brn * kernels[5 * 9 + 8] + biases[5]);
        outMat[6] =
            RULE(
                tln * kernels[6 * 9 + 0] + tcn * kernels[6 * 9 + 1] + trn * kernels[6 * 9 + 2] +
                mln * kernels[6 * 9 + 3] + mcn * kernels[6 * 9 + 4] + mrn * kernels[6 * 9 + 5] +
                bln * kernels[6 * 9 + 6] + bcn * kernels[6 * 9 + 7] + brn * kernels[6 * 9 + 8] + biases[6]);
        outMat[7] =
            RULE(
                tln * kernels[7 * 9 + 0] + tcn * kernels[7 * 9 + 1] + trn * kernels[7 * 9 + 2] +
                mln * kernels[7 * 9 + 3] + mcn * kernels[7 * 9 + 4] + mrn * kernels[7 * 9 + 5] +
                bln * kernels[7 * 9 + 6] + bcn * kernels[7 * 9 + 7] + brn * kernels[7 * 9 + 8] + biases[7]);

        }, tmpMat, 8);
}

void Anime4KCPP::CPU::CNNProcessor::conv1To8W(const cv::Mat& img, const double* kernels, const double* biases, cv::Mat& tmpMat)
{
    const int channels = 8;
    const int srcChannels = img.channels();
    const int lineStep = img.cols * srcChannels;
    changEachPixel1ToN(img, [&](const int i, const int j, ChanD outMat, LineW curLine) {
        const int orgJ = j / channels * srcChannels;
        const int jp = orgJ < (img.cols - 1)* srcChannels ? srcChannels : 0;
        const int jn = orgJ > srcChannels ? -srcChannels : 0;
        const LineW pLineData = i < img.rows - 1 ? curLine + lineStep : curLine;
        const LineW cLineData = curLine;
        const LineW nLineData = i > 0 ? curLine - lineStep : curLine;

        const PixelW tl = nLineData + orgJ + jn, tc = nLineData + orgJ, tr = nLineData + orgJ + jp;
        const PixelW ml = cLineData + orgJ + jn, mc = cLineData + orgJ, mr = cLineData + orgJ + jp;
        const PixelW bl = pLineData + orgJ + jn, bc = pLineData + orgJ, br = pLineData + orgJ + jp;

        const double tln = NORMW(tl[Y]);
        const double tcn = NORMW(tc[Y]);
        const double trn = NORMW(tr[Y]);
        const double mln = NORMW(ml[Y]);
        const double mcn = NORMW(mc[Y]);
        const double mrn = NORMW(mr[Y]);
        const double bln = NORMW(bl[Y]);
        const double bcn = NORMW(bc[Y]);
        const double brn = NORMW(br[Y]);

        outMat[0] =
            RULE(
                tln * kernels[0 * 9 + 0] + tcn * kernels[0 * 9 + 1] + trn * kernels[0 * 9 + 2] +
                mln * kernels[0 * 9 + 3] + mcn * kernels[0 * 9 + 4] + mrn * kernels[0 * 9 + 5] +
                bln * kernels[0 * 9 + 6] + bcn * kernels[0 * 9 + 7] + brn * kernels[0 * 9 + 8] + biases[0]);
        outMat[1] =
            RULE(
                tln * kernels[1 * 9 + 0] + tcn * kernels[1 * 9 + 1] + trn * kernels[1 * 9 + 2] +
                mln * kernels[1 * 9 + 3] + mcn * kernels[1 * 9 + 4] + mrn * kernels[1 * 9 + 5] +
                bln * kernels[1 * 9 + 6] + bcn * kernels[1 * 9 + 7] + brn * kernels[1 * 9 + 8] + biases[1]);
        outMat[2] =
            RULE(
                tln * kernels[2 * 9 + 0] + tcn * kernels[2 * 9 + 1] + trn * kernels[2 * 9 + 2] +
                mln * kernels[2 * 9 + 3] + mcn * kernels[2 * 9 + 4] + mrn * kernels[2 * 9 + 5] +
                bln * kernels[2 * 9 + 6] + bcn * kernels[2 * 9 + 7] + brn * kernels[2 * 9 + 8] + biases[2]);
        outMat[3] =
            RULE(
                tln * kernels[3 * 9 + 0] + tcn * kernels[3 * 9 + 1] + trn * kernels[3 * 9 + 2] +
                mln * kernels[3 * 9 + 3] + mcn * kernels[3 * 9 + 4] + mrn * kernels[3 * 9 + 5] +
                bln * kernels[3 * 9 + 6] + bcn * kernels[3 * 9 + 7] + brn * kernels[3 * 9 + 8] + biases[3]);
        outMat[4] =
            RULE(
                tln * kernels[4 * 9 + 0] + tcn * kernels[4 * 9 + 1] + trn * kernels[4 * 9 + 2] +
                mln * kernels[4 * 9 + 3] + mcn * kernels[4 * 9 + 4] + mrn * kernels[4 * 9 + 5] +
                bln * kernels[4 * 9 + 6] + bcn * kernels[4 * 9 + 7] + brn * kernels[4 * 9 + 8] + biases[4]);
        outMat[5] =
            RULE(
                tln * kernels[5 * 9 + 0] + tcn * kernels[5 * 9 + 1] + trn * kernels[5 * 9 + 2] +
                mln * kernels[5 * 9 + 3] + mcn * kernels[5 * 9 + 4] + mrn * kernels[5 * 9 + 5] +
                bln * kernels[5 * 9 + 6] + bcn * kernels[5 * 9 + 7] + brn * kernels[5 * 9 + 8] + biases[5]);
        outMat[6] =
            RULE(
                tln * kernels[6 * 9 + 0] + tcn * kernels[6 * 9 + 1] + trn * kernels[6 * 9 + 2] +
                mln * kernels[6 * 9 + 3] + mcn * kernels[6 * 9 + 4] + mrn * kernels[6 * 9 + 5] +
                bln * kernels[6 * 9 + 6] + bcn * kernels[6 * 9 + 7] + brn * kernels[6 * 9 + 8] + biases[6]);
        outMat[7] =
            RULE(
                tln * kernels[7 * 9 + 0] + tcn * kernels[7 * 9 + 1] + trn * kernels[7 * 9 + 2] +
                mln * kernels[7 * 9 + 3] + mcn * kernels[7 * 9 + 4] + mrn * kernels[7 * 9 + 5] +
                bln * kernels[7 * 9 + 6] + bcn * kernels[7 * 9 + 7] + brn * kernels[7 * 9 + 8] + biases[7]);

        }, tmpMat, 8);
}

void Anime4KCPP::CPU::CNNProcessor::conv1To8F(const cv::Mat& img, const double* kernels, const double* biases, cv::Mat& tmpMat)
{
    const int channels = 8;
    const int srcChannels = img.channels();
    const int lineStep = img.cols * srcChannels;
    changEachPixel1ToN(img, [&](const int i, const int j, ChanD outMat, LineF curLine) {
        const int orgJ = j / channels * srcChannels;
        const int jp = orgJ < (img.cols - 1)* srcChannels ? srcChannels : 0;
        const int jn = orgJ > srcChannels ? -srcChannels : 0;
        const LineF pLineData = i < img.rows - 1 ? curLine + lineStep : curLine;
        const LineF cLineData = curLine;
        const LineF nLineData = i > 0 ? curLine - lineStep : curLine;

        const PixelF tl = nLineData + orgJ + jn, tc = nLineData + orgJ, tr = nLineData + orgJ + jp;
        const PixelF ml = cLineData + orgJ + jn, mc = cLineData + orgJ, mr = cLineData + orgJ + jp;
        const PixelF bl = pLineData + orgJ + jn, bc = pLineData + orgJ, br = pLineData + orgJ + jp;

        const double tln = tl[Y];
        const double tcn = tc[Y];
        const double trn = tr[Y];
        const double mln = ml[Y];
        const double mcn = mc[Y];
        const double mrn = mr[Y];
        const double bln = bl[Y];
        const double bcn = bc[Y];
        const double brn = br[Y];

        outMat[0] =
            RULE(
                tln * kernels[0 * 9 + 0] + tcn * kernels[0 * 9 + 1] + trn * kernels[0 * 9 + 2] +
                mln * kernels[0 * 9 + 3] + mcn * kernels[0 * 9 + 4] + mrn * kernels[0 * 9 + 5] +
                bln * kernels[0 * 9 + 6] + bcn * kernels[0 * 9 + 7] + brn * kernels[0 * 9 + 8] + biases[0]);
        outMat[1] =
            RULE(
                tln * kernels[1 * 9 + 0] + tcn * kernels[1 * 9 + 1] + trn * kernels[1 * 9 + 2] +
                mln * kernels[1 * 9 + 3] + mcn * kernels[1 * 9 + 4] + mrn * kernels[1 * 9 + 5] +
                bln * kernels[1 * 9 + 6] + bcn * kernels[1 * 9 + 7] + brn * kernels[1 * 9 + 8] + biases[1]);
        outMat[2] =
            RULE(
                tln * kernels[2 * 9 + 0] + tcn * kernels[2 * 9 + 1] + trn * kernels[2 * 9 + 2] +
                mln * kernels[2 * 9 + 3] + mcn * kernels[2 * 9 + 4] + mrn * kernels[2 * 9 + 5] +
                bln * kernels[2 * 9 + 6] + bcn * kernels[2 * 9 + 7] + brn * kernels[2 * 9 + 8] + biases[2]);
        outMat[3] =
            RULE(
                tln * kernels[3 * 9 + 0] + tcn * kernels[3 * 9 + 1] + trn * kernels[3 * 9 + 2] +
                mln * kernels[3 * 9 + 3] + mcn * kernels[3 * 9 + 4] + mrn * kernels[3 * 9 + 5] +
                bln * kernels[3 * 9 + 6] + bcn * kernels[3 * 9 + 7] + brn * kernels[3 * 9 + 8] + biases[3]);
        outMat[4] =
            RULE(
                tln * kernels[4 * 9 + 0] + tcn * kernels[4 * 9 + 1] + trn * kernels[4 * 9 + 2] +
                mln * kernels[4 * 9 + 3] + mcn * kernels[4 * 9 + 4] + mrn * kernels[4 * 9 + 5] +
                bln * kernels[4 * 9 + 6] + bcn * kernels[4 * 9 + 7] + brn * kernels[4 * 9 + 8] + biases[4]);
        outMat[5] =
            RULE(
                tln * kernels[5 * 9 + 0] + tcn * kernels[5 * 9 + 1] + trn * kernels[5 * 9 + 2] +
                mln * kernels[5 * 9 + 3] + mcn * kernels[5 * 9 + 4] + mrn * kernels[5 * 9 + 5] +
                bln * kernels[5 * 9 + 6] + bcn * kernels[5 * 9 + 7] + brn * kernels[5 * 9 + 8] + biases[5]);
        outMat[6] =
            RULE(
                tln * kernels[6 * 9 + 0] + tcn * kernels[6 * 9 + 1] + trn * kernels[6 * 9 + 2] +
                mln * kernels[6 * 9 + 3] + mcn * kernels[6 * 9 + 4] + mrn * kernels[6 * 9 + 5] +
                bln * kernels[6 * 9 + 6] + bcn * kernels[6 * 9 + 7] + brn * kernels[6 * 9 + 8] + biases[6]);
        outMat[7] =
            RULE(
                tln * kernels[7 * 9 + 0] + tcn * kernels[7 * 9 + 1] + trn * kernels[7 * 9 + 2] +
                mln * kernels[7 * 9 + 3] + mcn * kernels[7 * 9 + 4] + mrn * kernels[7 * 9 + 5] +
                bln * kernels[7 * 9 + 6] + bcn * kernels[7 * 9 + 7] + brn * kernels[7 * 9 + 8] + biases[7]);

        }, tmpMat, 8);
}

void Anime4KCPP::CPU::CNNProcessor::conv8To8(const double* kernels, const double* biases, cv::Mat& tmpMat)
{
    const int channels = 8;
    const int lineStep = tmpMat.cols * channels;
    changEachPixelNToN([&](const int i, const int j, ChanD outMat, LineD curLine) {
        const int jp = j < (tmpMat.cols - 1)* channels ? channels : 0;
        const int jn = j > channels ? -channels : 0;
        const LineD pLineData = i < tmpMat.rows - 1 ? curLine + lineStep : curLine;
        const LineD cLineData = curLine;
        const LineD nLineData = i > 0 ? curLine - lineStep : curLine;

        const PixelD tl = nLineData + j + jn, tc = nLineData + j, tr = nLineData + j + jp;
        const PixelD ml = cLineData + j + jn, mc = cLineData + j, mr = cLineData + j + jp;
        const PixelD bl = pLineData + j + jn, bc = pLineData + j, br = pLineData + j + jp;

        double c1 =
            tl[0] * kernels[0 * 72 + 0 * 9 + 0] + tc[0] * kernels[0 * 72 + 0 * 9 + 1] + tr[0] * kernels[0 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[0 * 72 + 0 * 9 + 3] + mc[0] * kernels[0 * 72 + 0 * 9 + 4] + mr[0] * kernels[0 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[0 * 72 + 0 * 9 + 6] + bc[0] * kernels[0 * 72 + 0 * 9 + 7] + br[0] * kernels[0 * 72 + 0 * 9 + 8];

        double c2 =
            tl[1] * kernels[0 * 72 + 1 * 9 + 0] + tc[1] * kernels[0 * 72 + 1 * 9 + 1] + tr[1] * kernels[0 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[0 * 72 + 1 * 9 + 3] + mc[1] * kernels[0 * 72 + 1 * 9 + 4] + mr[1] * kernels[0 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[0 * 72 + 1 * 9 + 6] + bc[1] * kernels[0 * 72 + 1 * 9 + 7] + br[1] * kernels[0 * 72 + 1 * 9 + 8];

        double c3 =
            tl[2] * kernels[0 * 72 + 2 * 9 + 0] + tc[2] * kernels[0 * 72 + 2 * 9 + 1] + tr[2] * kernels[0 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[0 * 72 + 2 * 9 + 3] + mc[2] * kernels[0 * 72 + 2 * 9 + 4] + mr[2] * kernels[0 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[0 * 72 + 2 * 9 + 6] + bc[2] * kernels[0 * 72 + 2 * 9 + 7] + br[2] * kernels[0 * 72 + 2 * 9 + 8];

        double c4 =
            tl[3] * kernels[0 * 72 + 3 * 9 + 0] + tc[3] * kernels[0 * 72 + 3 * 9 + 1] + tr[3] * kernels[0 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[0 * 72 + 3 * 9 + 3] + mc[3] * kernels[0 * 72 + 3 * 9 + 4] + mr[3] * kernels[0 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[0 * 72 + 3 * 9 + 6] + bc[3] * kernels[0 * 72 + 3 * 9 + 7] + br[3] * kernels[0 * 72 + 3 * 9 + 8];

        double c5 =
            tl[4] * kernels[0 * 72 + 4 * 9 + 0] + tc[4] * kernels[0 * 72 + 4 * 9 + 1] + tr[4] * kernels[0 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[0 * 72 + 4 * 9 + 3] + mc[4] * kernels[0 * 72 + 4 * 9 + 4] + mr[4] * kernels[0 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[0 * 72 + 4 * 9 + 6] + bc[4] * kernels[0 * 72 + 4 * 9 + 7] + br[4] * kernels[0 * 72 + 4 * 9 + 8];

        double c6 =
            tl[5] * kernels[0 * 72 + 5 * 9 + 0] + tc[5] * kernels[0 * 72 + 5 * 9 + 1] + tr[5] * kernels[0 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[0 * 72 + 5 * 9 + 3] + mc[5] * kernels[0 * 72 + 5 * 9 + 4] + mr[5] * kernels[0 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[0 * 72 + 5 * 9 + 6] + bc[5] * kernels[0 * 72 + 5 * 9 + 7] + br[5] * kernels[0 * 72 + 5 * 9 + 8];

        double c7 =
            tl[6] * kernels[0 * 72 + 6 * 9 + 0] + tc[6] * kernels[0 * 72 + 6 * 9 + 1] + tr[6] * kernels[0 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[0 * 72 + 6 * 9 + 3] + mc[6] * kernels[0 * 72 + 6 * 9 + 4] + mr[6] * kernels[0 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[0 * 72 + 6 * 9 + 6] + bc[6] * kernels[0 * 72 + 6 * 9 + 7] + br[6] * kernels[0 * 72 + 6 * 9 + 8];

        double c8 =
            tl[7] * kernels[0 * 72 + 7 * 9 + 0] + tc[7] * kernels[0 * 72 + 7 * 9 + 1] + tr[7] * kernels[0 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[0 * 72 + 7 * 9 + 3] + mc[7] * kernels[0 * 72 + 7 * 9 + 4] + mr[7] * kernels[0 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[0 * 72 + 7 * 9 + 6] + bc[7] * kernels[0 * 72 + 7 * 9 + 7] + br[7] * kernels[0 * 72 + 7 * 9 + 8];

        outMat[0] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[0]);

        c1 =
            tl[0] * kernels[1 * 72 + 0 * 9 + 0] + tc[0] * kernels[1 * 72 + 0 * 9 + 1] + tr[0] * kernels[1 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[1 * 72 + 0 * 9 + 3] + mc[0] * kernels[1 * 72 + 0 * 9 + 4] + mr[0] * kernels[1 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[1 * 72 + 0 * 9 + 6] + bc[0] * kernels[1 * 72 + 0 * 9 + 7] + br[0] * kernels[1 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[1 * 72 + 1 * 9 + 0] + tc[1] * kernels[1 * 72 + 1 * 9 + 1] + tr[1] * kernels[1 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[1 * 72 + 1 * 9 + 3] + mc[1] * kernels[1 * 72 + 1 * 9 + 4] + mr[1] * kernels[1 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[1 * 72 + 1 * 9 + 6] + bc[1] * kernels[1 * 72 + 1 * 9 + 7] + br[1] * kernels[1 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[1 * 72 + 2 * 9 + 0] + tc[2] * kernels[1 * 72 + 2 * 9 + 1] + tr[2] * kernels[1 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[1 * 72 + 2 * 9 + 3] + mc[2] * kernels[1 * 72 + 2 * 9 + 4] + mr[2] * kernels[1 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[1 * 72 + 2 * 9 + 6] + bc[2] * kernels[1 * 72 + 2 * 9 + 7] + br[2] * kernels[1 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[1 * 72 + 3 * 9 + 0] + tc[3] * kernels[1 * 72 + 3 * 9 + 1] + tr[3] * kernels[1 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[1 * 72 + 3 * 9 + 3] + mc[3] * kernels[1 * 72 + 3 * 9 + 4] + mr[3] * kernels[1 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[1 * 72 + 3 * 9 + 6] + bc[3] * kernels[1 * 72 + 3 * 9 + 7] + br[3] * kernels[1 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[1 * 72 + 4 * 9 + 0] + tc[4] * kernels[1 * 72 + 4 * 9 + 1] + tr[4] * kernels[1 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[1 * 72 + 4 * 9 + 3] + mc[4] * kernels[1 * 72 + 4 * 9 + 4] + mr[4] * kernels[1 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[1 * 72 + 4 * 9 + 6] + bc[4] * kernels[1 * 72 + 4 * 9 + 7] + br[4] * kernels[1 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[1 * 72 + 5 * 9 + 0] + tc[5] * kernels[1 * 72 + 5 * 9 + 1] + tr[5] * kernels[1 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[1 * 72 + 5 * 9 + 3] + mc[5] * kernels[1 * 72 + 5 * 9 + 4] + mr[5] * kernels[1 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[1 * 72 + 5 * 9 + 6] + bc[5] * kernels[1 * 72 + 5 * 9 + 7] + br[5] * kernels[1 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[1 * 72 + 6 * 9 + 0] + tc[6] * kernels[1 * 72 + 6 * 9 + 1] + tr[6] * kernels[1 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[1 * 72 + 6 * 9 + 3] + mc[6] * kernels[1 * 72 + 6 * 9 + 4] + mr[6] * kernels[1 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[1 * 72 + 6 * 9 + 6] + bc[6] * kernels[1 * 72 + 6 * 9 + 7] + br[6] * kernels[1 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[1 * 72 + 7 * 9 + 0] + tc[7] * kernels[1 * 72 + 7 * 9 + 1] + tr[7] * kernels[1 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[1 * 72 + 7 * 9 + 3] + mc[7] * kernels[1 * 72 + 7 * 9 + 4] + mr[7] * kernels[1 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[1 * 72 + 7 * 9 + 6] + bc[7] * kernels[1 * 72 + 7 * 9 + 7] + br[7] * kernels[1 * 72 + 7 * 9 + 8];

        outMat[1] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[1]);

        c1 =
            tl[0] * kernels[2 * 72 + 0 * 9 + 0] + tc[0] * kernels[2 * 72 + 0 * 9 + 1] + tr[0] * kernels[2 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[2 * 72 + 0 * 9 + 3] + mc[0] * kernels[2 * 72 + 0 * 9 + 4] + mr[0] * kernels[2 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[2 * 72 + 0 * 9 + 6] + bc[0] * kernels[2 * 72 + 0 * 9 + 7] + br[0] * kernels[2 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[2 * 72 + 1 * 9 + 0] + tc[1] * kernels[2 * 72 + 1 * 9 + 1] + tr[1] * kernels[2 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[2 * 72 + 1 * 9 + 3] + mc[1] * kernels[2 * 72 + 1 * 9 + 4] + mr[1] * kernels[2 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[2 * 72 + 1 * 9 + 6] + bc[1] * kernels[2 * 72 + 1 * 9 + 7] + br[1] * kernels[2 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[2 * 72 + 2 * 9 + 0] + tc[2] * kernels[2 * 72 + 2 * 9 + 1] + tr[2] * kernels[2 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[2 * 72 + 2 * 9 + 3] + mc[2] * kernels[2 * 72 + 2 * 9 + 4] + mr[2] * kernels[2 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[2 * 72 + 2 * 9 + 6] + bc[2] * kernels[2 * 72 + 2 * 9 + 7] + br[2] * kernels[2 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[2 * 72 + 3 * 9 + 0] + tc[3] * kernels[2 * 72 + 3 * 9 + 1] + tr[3] * kernels[2 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[2 * 72 + 3 * 9 + 3] + mc[3] * kernels[2 * 72 + 3 * 9 + 4] + mr[3] * kernels[2 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[2 * 72 + 3 * 9 + 6] + bc[3] * kernels[2 * 72 + 3 * 9 + 7] + br[3] * kernels[2 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[2 * 72 + 4 * 9 + 0] + tc[4] * kernels[2 * 72 + 4 * 9 + 1] + tr[4] * kernels[2 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[2 * 72 + 4 * 9 + 3] + mc[4] * kernels[2 * 72 + 4 * 9 + 4] + mr[4] * kernels[2 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[2 * 72 + 4 * 9 + 6] + bc[4] * kernels[2 * 72 + 4 * 9 + 7] + br[4] * kernels[2 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[2 * 72 + 5 * 9 + 0] + tc[5] * kernels[2 * 72 + 5 * 9 + 1] + tr[5] * kernels[2 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[2 * 72 + 5 * 9 + 3] + mc[5] * kernels[2 * 72 + 5 * 9 + 4] + mr[5] * kernels[2 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[2 * 72 + 5 * 9 + 6] + bc[5] * kernels[2 * 72 + 5 * 9 + 7] + br[5] * kernels[2 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[2 * 72 + 6 * 9 + 0] + tc[6] * kernels[2 * 72 + 6 * 9 + 1] + tr[6] * kernels[2 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[2 * 72 + 6 * 9 + 3] + mc[6] * kernels[2 * 72 + 6 * 9 + 4] + mr[6] * kernels[2 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[2 * 72 + 6 * 9 + 6] + bc[6] * kernels[2 * 72 + 6 * 9 + 7] + br[6] * kernels[2 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[2 * 72 + 7 * 9 + 0] + tc[7] * kernels[2 * 72 + 7 * 9 + 1] + tr[7] * kernels[2 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[2 * 72 + 7 * 9 + 3] + mc[7] * kernels[2 * 72 + 7 * 9 + 4] + mr[7] * kernels[2 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[2 * 72 + 7 * 9 + 6] + bc[7] * kernels[2 * 72 + 7 * 9 + 7] + br[7] * kernels[2 * 72 + 7 * 9 + 8];

        outMat[2] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[2]);

        c1 =
            tl[0] * kernels[3 * 72 + 0 * 9 + 0] + tc[0] * kernels[3 * 72 + 0 * 9 + 1] + tr[0] * kernels[3 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[3 * 72 + 0 * 9 + 3] + mc[0] * kernels[3 * 72 + 0 * 9 + 4] + mr[0] * kernels[3 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[3 * 72 + 0 * 9 + 6] + bc[0] * kernels[3 * 72 + 0 * 9 + 7] + br[0] * kernels[3 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[3 * 72 + 1 * 9 + 0] + tc[1] * kernels[3 * 72 + 1 * 9 + 1] + tr[1] * kernels[3 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[3 * 72 + 1 * 9 + 3] + mc[1] * kernels[3 * 72 + 1 * 9 + 4] + mr[1] * kernels[3 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[3 * 72 + 1 * 9 + 6] + bc[1] * kernels[3 * 72 + 1 * 9 + 7] + br[1] * kernels[3 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[3 * 72 + 2 * 9 + 0] + tc[2] * kernels[3 * 72 + 2 * 9 + 1] + tr[2] * kernels[3 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[3 * 72 + 2 * 9 + 3] + mc[2] * kernels[3 * 72 + 2 * 9 + 4] + mr[2] * kernels[3 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[3 * 72 + 2 * 9 + 6] + bc[2] * kernels[3 * 72 + 2 * 9 + 7] + br[2] * kernels[3 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[3 * 72 + 3 * 9 + 0] + tc[3] * kernels[3 * 72 + 3 * 9 + 1] + tr[3] * kernels[3 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[3 * 72 + 3 * 9 + 3] + mc[3] * kernels[3 * 72 + 3 * 9 + 4] + mr[3] * kernels[3 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[3 * 72 + 3 * 9 + 6] + bc[3] * kernels[3 * 72 + 3 * 9 + 7] + br[3] * kernels[3 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[3 * 72 + 4 * 9 + 0] + tc[4] * kernels[3 * 72 + 4 * 9 + 1] + tr[4] * kernels[3 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[3 * 72 + 4 * 9 + 3] + mc[4] * kernels[3 * 72 + 4 * 9 + 4] + mr[4] * kernels[3 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[3 * 72 + 4 * 9 + 6] + bc[4] * kernels[3 * 72 + 4 * 9 + 7] + br[4] * kernels[3 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[3 * 72 + 5 * 9 + 0] + tc[5] * kernels[3 * 72 + 5 * 9 + 1] + tr[5] * kernels[3 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[3 * 72 + 5 * 9 + 3] + mc[5] * kernels[3 * 72 + 5 * 9 + 4] + mr[5] * kernels[3 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[3 * 72 + 5 * 9 + 6] + bc[5] * kernels[3 * 72 + 5 * 9 + 7] + br[5] * kernels[3 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[3 * 72 + 6 * 9 + 0] + tc[6] * kernels[3 * 72 + 6 * 9 + 1] + tr[6] * kernels[3 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[3 * 72 + 6 * 9 + 3] + mc[6] * kernels[3 * 72 + 6 * 9 + 4] + mr[6] * kernels[3 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[3 * 72 + 6 * 9 + 6] + bc[6] * kernels[3 * 72 + 6 * 9 + 7] + br[6] * kernels[3 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[3 * 72 + 7 * 9 + 0] + tc[7] * kernels[3 * 72 + 7 * 9 + 1] + tr[7] * kernels[3 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[3 * 72 + 7 * 9 + 3] + mc[7] * kernels[3 * 72 + 7 * 9 + 4] + mr[7] * kernels[3 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[3 * 72 + 7 * 9 + 6] + bc[7] * kernels[3 * 72 + 7 * 9 + 7] + br[7] * kernels[3 * 72 + 7 * 9 + 8];

        outMat[3] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[3]);

        c1 =
            tl[0] * kernels[4 * 72 + 0 * 9 + 0] + tc[0] * kernels[4 * 72 + 0 * 9 + 1] + tr[0] * kernels[4 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[4 * 72 + 0 * 9 + 3] + mc[0] * kernels[4 * 72 + 0 * 9 + 4] + mr[0] * kernels[4 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[4 * 72 + 0 * 9 + 6] + bc[0] * kernels[4 * 72 + 0 * 9 + 7] + br[0] * kernels[4 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[4 * 72 + 1 * 9 + 0] + tc[1] * kernels[4 * 72 + 1 * 9 + 1] + tr[1] * kernels[4 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[4 * 72 + 1 * 9 + 3] + mc[1] * kernels[4 * 72 + 1 * 9 + 4] + mr[1] * kernels[4 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[4 * 72 + 1 * 9 + 6] + bc[1] * kernels[4 * 72 + 1 * 9 + 7] + br[1] * kernels[4 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[4 * 72 + 2 * 9 + 0] + tc[2] * kernels[4 * 72 + 2 * 9 + 1] + tr[2] * kernels[4 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[4 * 72 + 2 * 9 + 3] + mc[2] * kernels[4 * 72 + 2 * 9 + 4] + mr[2] * kernels[4 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[4 * 72 + 2 * 9 + 6] + bc[2] * kernels[4 * 72 + 2 * 9 + 7] + br[2] * kernels[4 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[4 * 72 + 3 * 9 + 0] + tc[3] * kernels[4 * 72 + 3 * 9 + 1] + tr[3] * kernels[4 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[4 * 72 + 3 * 9 + 3] + mc[3] * kernels[4 * 72 + 3 * 9 + 4] + mr[3] * kernels[4 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[4 * 72 + 3 * 9 + 6] + bc[3] * kernels[4 * 72 + 3 * 9 + 7] + br[3] * kernels[4 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[4 * 72 + 4 * 9 + 0] + tc[4] * kernels[4 * 72 + 4 * 9 + 1] + tr[4] * kernels[4 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[4 * 72 + 4 * 9 + 3] + mc[4] * kernels[4 * 72 + 4 * 9 + 4] + mr[4] * kernels[4 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[4 * 72 + 4 * 9 + 6] + bc[4] * kernels[4 * 72 + 4 * 9 + 7] + br[4] * kernels[4 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[4 * 72 + 5 * 9 + 0] + tc[5] * kernels[4 * 72 + 5 * 9 + 1] + tr[5] * kernels[4 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[4 * 72 + 5 * 9 + 3] + mc[5] * kernels[4 * 72 + 5 * 9 + 4] + mr[5] * kernels[4 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[4 * 72 + 5 * 9 + 6] + bc[5] * kernels[4 * 72 + 5 * 9 + 7] + br[5] * kernels[4 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[4 * 72 + 6 * 9 + 0] + tc[6] * kernels[4 * 72 + 6 * 9 + 1] + tr[6] * kernels[4 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[4 * 72 + 6 * 9 + 3] + mc[6] * kernels[4 * 72 + 6 * 9 + 4] + mr[6] * kernels[4 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[4 * 72 + 6 * 9 + 6] + bc[6] * kernels[4 * 72 + 6 * 9 + 7] + br[6] * kernels[4 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[4 * 72 + 7 * 9 + 0] + tc[7] * kernels[4 * 72 + 7 * 9 + 1] + tr[7] * kernels[4 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[4 * 72 + 7 * 9 + 3] + mc[7] * kernels[4 * 72 + 7 * 9 + 4] + mr[7] * kernels[4 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[4 * 72 + 7 * 9 + 6] + bc[7] * kernels[4 * 72 + 7 * 9 + 7] + br[7] * kernels[4 * 72 + 7 * 9 + 8];

        outMat[4] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[4]);

        c1 =
            tl[0] * kernels[5 * 72 + 0 * 9 + 0] + tc[0] * kernels[5 * 72 + 0 * 9 + 1] + tr[0] * kernels[5 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[5 * 72 + 0 * 9 + 3] + mc[0] * kernels[5 * 72 + 0 * 9 + 4] + mr[0] * kernels[5 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[5 * 72 + 0 * 9 + 6] + bc[0] * kernels[5 * 72 + 0 * 9 + 7] + br[0] * kernels[5 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[5 * 72 + 1 * 9 + 0] + tc[1] * kernels[5 * 72 + 1 * 9 + 1] + tr[1] * kernels[5 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[5 * 72 + 1 * 9 + 3] + mc[1] * kernels[5 * 72 + 1 * 9 + 4] + mr[1] * kernels[5 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[5 * 72 + 1 * 9 + 6] + bc[1] * kernels[5 * 72 + 1 * 9 + 7] + br[1] * kernels[5 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[5 * 72 + 2 * 9 + 0] + tc[2] * kernels[5 * 72 + 2 * 9 + 1] + tr[2] * kernels[5 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[5 * 72 + 2 * 9 + 3] + mc[2] * kernels[5 * 72 + 2 * 9 + 4] + mr[2] * kernels[5 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[5 * 72 + 2 * 9 + 6] + bc[2] * kernels[5 * 72 + 2 * 9 + 7] + br[2] * kernels[5 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[5 * 72 + 3 * 9 + 0] + tc[3] * kernels[5 * 72 + 3 * 9 + 1] + tr[3] * kernels[5 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[5 * 72 + 3 * 9 + 3] + mc[3] * kernels[5 * 72 + 3 * 9 + 4] + mr[3] * kernels[5 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[5 * 72 + 3 * 9 + 6] + bc[3] * kernels[5 * 72 + 3 * 9 + 7] + br[3] * kernels[5 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[5 * 72 + 4 * 9 + 0] + tc[4] * kernels[5 * 72 + 4 * 9 + 1] + tr[4] * kernels[5 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[5 * 72 + 4 * 9 + 3] + mc[4] * kernels[5 * 72 + 4 * 9 + 4] + mr[4] * kernels[5 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[5 * 72 + 4 * 9 + 6] + bc[4] * kernels[5 * 72 + 4 * 9 + 7] + br[4] * kernels[5 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[5 * 72 + 5 * 9 + 0] + tc[5] * kernels[5 * 72 + 5 * 9 + 1] + tr[5] * kernels[5 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[5 * 72 + 5 * 9 + 3] + mc[5] * kernels[5 * 72 + 5 * 9 + 4] + mr[5] * kernels[5 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[5 * 72 + 5 * 9 + 6] + bc[5] * kernels[5 * 72 + 5 * 9 + 7] + br[5] * kernels[5 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[5 * 72 + 6 * 9 + 0] + tc[6] * kernels[5 * 72 + 6 * 9 + 1] + tr[6] * kernels[5 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[5 * 72 + 6 * 9 + 3] + mc[6] * kernels[5 * 72 + 6 * 9 + 4] + mr[6] * kernels[5 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[5 * 72 + 6 * 9 + 6] + bc[6] * kernels[5 * 72 + 6 * 9 + 7] + br[6] * kernels[5 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[5 * 72 + 7 * 9 + 0] + tc[7] * kernels[5 * 72 + 7 * 9 + 1] + tr[7] * kernels[5 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[5 * 72 + 7 * 9 + 3] + mc[7] * kernels[5 * 72 + 7 * 9 + 4] + mr[7] * kernels[5 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[5 * 72 + 7 * 9 + 6] + bc[7] * kernels[5 * 72 + 7 * 9 + 7] + br[7] * kernels[5 * 72 + 7 * 9 + 8];

        outMat[5] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[5]);

        c1 =
            tl[0] * kernels[6 * 72 + 0 * 9 + 0] + tc[0] * kernels[6 * 72 + 0 * 9 + 1] + tr[0] * kernels[6 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[6 * 72 + 0 * 9 + 3] + mc[0] * kernels[6 * 72 + 0 * 9 + 4] + mr[0] * kernels[6 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[6 * 72 + 0 * 9 + 6] + bc[0] * kernels[6 * 72 + 0 * 9 + 7] + br[0] * kernels[6 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[6 * 72 + 1 * 9 + 0] + tc[1] * kernels[6 * 72 + 1 * 9 + 1] + tr[1] * kernels[6 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[6 * 72 + 1 * 9 + 3] + mc[1] * kernels[6 * 72 + 1 * 9 + 4] + mr[1] * kernels[6 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[6 * 72 + 1 * 9 + 6] + bc[1] * kernels[6 * 72 + 1 * 9 + 7] + br[1] * kernels[6 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[6 * 72 + 2 * 9 + 0] + tc[2] * kernels[6 * 72 + 2 * 9 + 1] + tr[2] * kernels[6 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[6 * 72 + 2 * 9 + 3] + mc[2] * kernels[6 * 72 + 2 * 9 + 4] + mr[2] * kernels[6 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[6 * 72 + 2 * 9 + 6] + bc[2] * kernels[6 * 72 + 2 * 9 + 7] + br[2] * kernels[6 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[6 * 72 + 3 * 9 + 0] + tc[3] * kernels[6 * 72 + 3 * 9 + 1] + tr[3] * kernels[6 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[6 * 72 + 3 * 9 + 3] + mc[3] * kernels[6 * 72 + 3 * 9 + 4] + mr[3] * kernels[6 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[6 * 72 + 3 * 9 + 6] + bc[3] * kernels[6 * 72 + 3 * 9 + 7] + br[3] * kernels[6 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[6 * 72 + 4 * 9 + 0] + tc[4] * kernels[6 * 72 + 4 * 9 + 1] + tr[4] * kernels[6 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[6 * 72 + 4 * 9 + 3] + mc[4] * kernels[6 * 72 + 4 * 9 + 4] + mr[4] * kernels[6 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[6 * 72 + 4 * 9 + 6] + bc[4] * kernels[6 * 72 + 4 * 9 + 7] + br[4] * kernels[6 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[6 * 72 + 5 * 9 + 0] + tc[5] * kernels[6 * 72 + 5 * 9 + 1] + tr[5] * kernels[6 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[6 * 72 + 5 * 9 + 3] + mc[5] * kernels[6 * 72 + 5 * 9 + 4] + mr[5] * kernels[6 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[6 * 72 + 5 * 9 + 6] + bc[5] * kernels[6 * 72 + 5 * 9 + 7] + br[5] * kernels[6 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[6 * 72 + 6 * 9 + 0] + tc[6] * kernels[6 * 72 + 6 * 9 + 1] + tr[6] * kernels[6 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[6 * 72 + 6 * 9 + 3] + mc[6] * kernels[6 * 72 + 6 * 9 + 4] + mr[6] * kernels[6 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[6 * 72 + 6 * 9 + 6] + bc[6] * kernels[6 * 72 + 6 * 9 + 7] + br[6] * kernels[6 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[6 * 72 + 7 * 9 + 0] + tc[7] * kernels[6 * 72 + 7 * 9 + 1] + tr[7] * kernels[6 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[6 * 72 + 7 * 9 + 3] + mc[7] * kernels[6 * 72 + 7 * 9 + 4] + mr[7] * kernels[6 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[6 * 72 + 7 * 9 + 6] + bc[7] * kernels[6 * 72 + 7 * 9 + 7] + br[7] * kernels[6 * 72 + 7 * 9 + 8];

        outMat[6] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[6]);

        c1 =
            tl[0] * kernels[7 * 72 + 0 * 9 + 0] + tc[0] * kernels[7 * 72 + 0 * 9 + 1] + tr[0] * kernels[7 * 72 + 0 * 9 + 2] +
            ml[0] * kernels[7 * 72 + 0 * 9 + 3] + mc[0] * kernels[7 * 72 + 0 * 9 + 4] + mr[0] * kernels[7 * 72 + 0 * 9 + 5] +
            bl[0] * kernels[7 * 72 + 0 * 9 + 6] + bc[0] * kernels[7 * 72 + 0 * 9 + 7] + br[0] * kernels[7 * 72 + 0 * 9 + 8];

        c2 =
            tl[1] * kernels[7 * 72 + 1 * 9 + 0] + tc[1] * kernels[7 * 72 + 1 * 9 + 1] + tr[1] * kernels[7 * 72 + 1 * 9 + 2] +
            ml[1] * kernels[7 * 72 + 1 * 9 + 3] + mc[1] * kernels[7 * 72 + 1 * 9 + 4] + mr[1] * kernels[7 * 72 + 1 * 9 + 5] +
            bl[1] * kernels[7 * 72 + 1 * 9 + 6] + bc[1] * kernels[7 * 72 + 1 * 9 + 7] + br[1] * kernels[7 * 72 + 1 * 9 + 8];

        c3 =
            tl[2] * kernels[7 * 72 + 2 * 9 + 0] + tc[2] * kernels[7 * 72 + 2 * 9 + 1] + tr[2] * kernels[7 * 72 + 2 * 9 + 2] +
            ml[2] * kernels[7 * 72 + 2 * 9 + 3] + mc[2] * kernels[7 * 72 + 2 * 9 + 4] + mr[2] * kernels[7 * 72 + 2 * 9 + 5] +
            bl[2] * kernels[7 * 72 + 2 * 9 + 6] + bc[2] * kernels[7 * 72 + 2 * 9 + 7] + br[2] * kernels[7 * 72 + 2 * 9 + 8];

        c4 =
            tl[3] * kernels[7 * 72 + 3 * 9 + 0] + tc[3] * kernels[7 * 72 + 3 * 9 + 1] + tr[3] * kernels[7 * 72 + 3 * 9 + 2] +
            ml[3] * kernels[7 * 72 + 3 * 9 + 3] + mc[3] * kernels[7 * 72 + 3 * 9 + 4] + mr[3] * kernels[7 * 72 + 3 * 9 + 5] +
            bl[3] * kernels[7 * 72 + 3 * 9 + 6] + bc[3] * kernels[7 * 72 + 3 * 9 + 7] + br[3] * kernels[7 * 72 + 3 * 9 + 8];

        c5 =
            tl[4] * kernels[7 * 72 + 4 * 9 + 0] + tc[4] * kernels[7 * 72 + 4 * 9 + 1] + tr[4] * kernels[7 * 72 + 4 * 9 + 2] +
            ml[4] * kernels[7 * 72 + 4 * 9 + 3] + mc[4] * kernels[7 * 72 + 4 * 9 + 4] + mr[4] * kernels[7 * 72 + 4 * 9 + 5] +
            bl[4] * kernels[7 * 72 + 4 * 9 + 6] + bc[4] * kernels[7 * 72 + 4 * 9 + 7] + br[4] * kernels[7 * 72 + 4 * 9 + 8];

        c6 =
            tl[5] * kernels[7 * 72 + 5 * 9 + 0] + tc[5] * kernels[7 * 72 + 5 * 9 + 1] + tr[5] * kernels[7 * 72 + 5 * 9 + 2] +
            ml[5] * kernels[7 * 72 + 5 * 9 + 3] + mc[5] * kernels[7 * 72 + 5 * 9 + 4] + mr[5] * kernels[7 * 72 + 5 * 9 + 5] +
            bl[5] * kernels[7 * 72 + 5 * 9 + 6] + bc[5] * kernels[7 * 72 + 5 * 9 + 7] + br[5] * kernels[7 * 72 + 5 * 9 + 8];

        c7 =
            tl[6] * kernels[7 * 72 + 6 * 9 + 0] + tc[6] * kernels[7 * 72 + 6 * 9 + 1] + tr[6] * kernels[7 * 72 + 6 * 9 + 2] +
            ml[6] * kernels[7 * 72 + 6 * 9 + 3] + mc[6] * kernels[7 * 72 + 6 * 9 + 4] + mr[6] * kernels[7 * 72 + 6 * 9 + 5] +
            bl[6] * kernels[7 * 72 + 6 * 9 + 6] + bc[6] * kernels[7 * 72 + 6 * 9 + 7] + br[6] * kernels[7 * 72 + 6 * 9 + 8];

        c8 =
            tl[7] * kernels[7 * 72 + 7 * 9 + 0] + tc[7] * kernels[7 * 72 + 7 * 9 + 1] + tr[7] * kernels[7 * 72 + 7 * 9 + 2] +
            ml[7] * kernels[7 * 72 + 7 * 9 + 3] + mc[7] * kernels[7 * 72 + 7 * 9 + 4] + mr[7] * kernels[7 * 72 + 7 * 9 + 5] +
            bl[7] * kernels[7 * 72 + 7 * 9 + 6] + bc[7] * kernels[7 * 72 + 7 * 9 + 7] + br[7] * kernels[7 * 72 + 7 * 9 + 8];

        outMat[7] = RULE(c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + biases[7]);

        }, tmpMat);
}

void Anime4KCPP::CPU::CNNProcessor::convTranspose8To1B(cv::Mat& img, const double* kernels, cv::Mat& tmpMat)
{
    changEachPixelNTo1(img, [&](const int i, const int j, ChanB outMat, LineD inMat) {
        const int index = ((i & 1) << 1) + (j & 1);

        //180 degree rotation for kernel
        //0 1  to  3 2
        //2 3      1 0

        const double luma = (
            inMat[0] * kernels[0 + index] +
            inMat[1] * kernels[4 + index] +
            inMat[2] * kernels[8 + index] +
            inMat[3] * kernels[12 + index] +
            inMat[4] * kernels[16 + index] +
            inMat[5] * kernels[20 + index] +
            inMat[6] * kernels[24 + index] +
            inMat[7] * kernels[28 + index]);

        *outMat = UNNORMB(luma);

        }, tmpMat);
}

void Anime4KCPP::CPU::CNNProcessor::convTranspose8To1W(cv::Mat& img, const double* kernels, cv::Mat& tmpMat)
{
    changEachPixelNTo1(img, [&](const int i, const int j, ChanW outMat, LineD inMat) {
        const int index = ((i & 1) << 1) + (j & 1);

        //180 degree rotation for kernel
        //0 1  to  3 2
        //2 3      1 0

        const double luma = (
            inMat[0] * kernels[0 + index] +
            inMat[1] * kernels[4 + index] +
            inMat[2] * kernels[8 + index] +
            inMat[3] * kernels[12 + index] +
            inMat[4] * kernels[16 + index] +
            inMat[5] * kernels[20 + index] +
            inMat[6] * kernels[24 + index] +
            inMat[7] * kernels[28 + index]);

        *outMat = UNNORMW(luma);

        }, tmpMat);
}

void Anime4KCPP::CPU::CNNProcessor::convTranspose8To1F(cv::Mat& img, const double* kernels, cv::Mat& tmpMat)
{
    changEachPixelNTo1(img, [&](const int i, const int j, ChanF outMat, LineD inMat) {
        const int index = ((i & 1) << 1) + (j & 1);

        //180 degree rotation for kernel
        //0 1  to  3 2
        //2 3      1 0

        const double luma = (
            inMat[0] * kernels[0 + index] +
            inMat[1] * kernels[4 + index] +
            inMat[2] * kernels[8 + index] +
            inMat[3] * kernels[12 + index] +
            inMat[4] * kernels[16 + index] +
            inMat[5] * kernels[20 + index] +
            inMat[6] * kernels[24 + index] +
            inMat[7] * kernels[28 + index]);

        *outMat = static_cast<float>(CLAMP(luma, 0.0, 1.0));

        }, tmpMat);
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixel1ToN(const cv::Mat& src,
    const std::function<void(int, int, ChanD, LineB)>&& callBack,
    cv::Mat& tmpMat, int outChannels)
{
    tmpMat.create(src.size(), CV_64FC(outChannels));

    const size_t srcChannels = src.channels();

    const int h = src.rows, w = src.cols;

    const int jMAX = w * outChannels;
    const size_t step = jMAX;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineB lineData = src.data + static_cast<size_t>(i) * static_cast<size_t>(w) * srcChannels;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmpMat.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += outChannels)
            callBack(i, j, tmpLineData + j, lineData);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineB lineData = src.data + static_cast<size_t>(i) * static_cast<size_t>(w) * srcChannels;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmpMat.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += outChannels)
            callBack(i, j, tmpLineData + j, lineData);
    }
#endif
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixel1ToN(const cv::Mat& src,
    const std::function<void(int, int, ChanD, LineW)>&& callBack,
    cv::Mat& tmpMat, int outChannels)
{
    tmpMat.create(src.size(), CV_64FC(outChannels));

    const size_t srcChannels = src.channels();

    const int h = src.rows, w = src.cols;

    const int jMAX = w * outChannels;
    const size_t step = jMAX;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineW lineData = reinterpret_cast<LineW>(src.data) + static_cast<size_t>(i) * static_cast<size_t>(w) * srcChannels;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmpMat.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += outChannels)
            callBack(i, j, tmpLineData + j, lineData);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineW lineData = reinterpret_cast<LineW>(src.data) + static_cast<size_t>(i) * static_cast<size_t>(w) * srcChannels;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmpMat.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += outChannels)
            callBack(i, j, tmpLineData + j, lineData);
    }
#endif
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixel1ToN(const cv::Mat& src,
    const std::function<void(int, int, ChanD, LineF)>&& callBack,
    cv::Mat& tmpMat, int outChannels)
{
    tmpMat.create(src.size(), CV_64FC(outChannels));

    const size_t srcChannels = src.channels();

    const int h = src.rows, w = src.cols;

    const int jMAX = w * outChannels;
    const size_t step = jMAX;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineF lineData = reinterpret_cast<LineF>(src.data) + static_cast<size_t>(i) * static_cast<size_t>(w) * srcChannels;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmpMat.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += outChannels)
            callBack(i, j, tmpLineData + j, lineData);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineF lineData = reinterpret_cast<LineF>(src.data) + static_cast<size_t>(i) * static_cast<size_t>(w) * srcChannels;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmpMat.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += outChannels)
            callBack(i, j, tmpLineData + j, lineData);
    }
#endif
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixelNToN(
    const std::function<void(int, int, ChanD, LineD)>&& callBack,
    cv::Mat& tmpMat)
{
    cv::Mat tmp;
    tmp.create(tmpMat.size(), tmpMat.type());

    const int h = tmpMat.rows, w = tmpMat.cols;

    const int channels = tmpMat.channels();
    const int jMAX = w * channels;
    const size_t step = jMAX;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i) * step;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmp.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += channels)
            callBack(i, j, tmpLineData + j, lineData);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i) * step;
        ChanD tmpLineData = reinterpret_cast<ChanD>(tmp.data) + static_cast<size_t>(i) * step;
        for (int j = 0; j < jMAX; j += channels)
            callBack(i, j, tmpLineData + j, lineData);
    }
#endif

    tmpMat = tmp;
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixelNTo1(cv::Mat& img,
    const std::function<void(int, int, ChanB, LineD)>&& callBack,
    const cv::Mat& tmpMat)
{
    cv::Mat tmp;
    const int h = 2 * tmpMat.rows, w = 2 * tmpMat.cols;
    tmp.create(h, w, CV_8UC1);

    const int jMAX = w;
    const size_t channels = tmpMat.channels();
    const size_t step = (jMAX >> 1) * channels;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i >> 1) * step;
        ChanB tmpLineData = tmp.data + static_cast<size_t>(i) * static_cast<size_t>(w);
        for (int j = 0; j < jMAX; j++)
            callBack(i, j, tmpLineData + j, lineData + static_cast<size_t>(j >> 1) * channels);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i >> 1) * step;
        ChanB tmpLineData = tmp.data + static_cast<size_t>(i) * static_cast<size_t>(w);
        for (int j = 0; j < jMAX; j++)
            callBack(i, j, tmpLineData + j, lineData + static_cast<size_t>(j >> 1) * channels);
    }
#endif

    img = tmp;
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixelNTo1(cv::Mat& img,
    const std::function<void(int, int, ChanW, LineD)>&& callBack,
    const cv::Mat& tmpMat)
{
    cv::Mat tmp;
    const int h = 2 * tmpMat.rows, w = 2 * tmpMat.cols;
    tmp.create(h, w, CV_16UC1);

    const int jMAX = w;
    const size_t channels = tmpMat.channels();
    const size_t step = (jMAX >> 1) * channels;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i >> 1) * step;
        ChanW tmpLineData = reinterpret_cast<ChanW>(tmp.data) + static_cast<size_t>(i) * static_cast<size_t>(w);
        for (int j = 0; j < jMAX; j++)
            callBack(i, j, tmpLineData + j, lineData + static_cast<size_t>(j >> 1) * channels);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i >> 1) * step;
        ChanW tmpLineData = reinterpret_cast<ChanW>(tmp.data) + static_cast<size_t>(i) * static_cast<size_t>(w);
        for (int j = 0; j < jMAX; j++)
            callBack(i, j, tmpLineData + j, lineData + static_cast<size_t>(j >> 1) * channels);
    }
#endif

    img = tmp;
}

void Anime4KCPP::CPU::CNNProcessor::changEachPixelNTo1(cv::Mat& img,
    const std::function<void(int, int, ChanF, LineD)>&& callBack,
    const cv::Mat& tmpMat)
{
    cv::Mat tmp;
    const int h = 2 * tmpMat.rows, w = 2 * tmpMat.cols;
    tmp.create(h, w, CV_32FC1);

    const int jMAX = w;
    const size_t channels = tmpMat.channels();
    const size_t step = (jMAX >> 1) * channels;

#if defined(_MSC_VER) || defined(USE_TBB)
    Parallel::parallel_for(0, h, [&](int i) {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i >> 1) * step;
        ChanF tmpLineData = reinterpret_cast<ChanF>(tmp.data) + static_cast<size_t>(i) * static_cast<size_t>(w);
        for (int j = 0; j < jMAX; j++)
            callBack(i, j, tmpLineData + j, lineData + static_cast<size_t>(j >> 1) * channels);
        });
#else
#pragma omp parallel for
    for (int i = 0; i < h; i++)
    {
        LineD lineData = reinterpret_cast<LineD>(tmpMat.data) + static_cast<size_t>(i >> 1) * step;
        ChanF tmpLineData = reinterpret_cast<ChanF>(tmp.data) + static_cast<size_t>(i) * static_cast<size_t>(w);
        for (int j = 0; j < jMAX; j++)
            callBack(i, j, tmpLineData + j, lineData + static_cast<size_t>(j >> 1) * channels);
    }
#endif

    img = tmp;
}
