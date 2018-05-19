#include "gtest/gtest.h"
#include "../matrix/sparse_matrix.h"
#include "../svd/sparse_svd.h"
#include "../qr/sparse_qr.h"

class qr_test : public ::testing::Test {
  protected:
    double vals[20] = {
      -0.24343752 ,-0.80608223 ,-0.09415779 ,-0.46258525,
      -0.1006011  ,-0.09249646 ,-0.92595241 , 0.28992492,
      -0.449498   , 0.58024157 ,-0.24633202 ,-0.57421492,
      -0.39125826 ,-0.05484052 , 0.16592905 ,-0.22590442,
      -0.75860801 ,-0.04458787 , 0.21338822 , 0.5667476 
    };
    cv::Mat* in;
    qr_test() {}

    virtual ~qr_test() {}

    virtual void SetUp() {
      in = new cv::Mat(5,5,CV_64F);
	  	cv::randu(*in, 0.0, 100.0);
    }

    virtual void TearDown() {
    }
  };

TEST_F(qr_test, gram_schmidt) {
  cv::Mat in_ = in->clone();
  QR_dense qr(in_);
  qr.gram_schmidt();
  cv::Mat d = (in_ - qr.Q*qr.R);
  double d_val = cv::norm(d);
  EXPECT_NEAR(d_val, 0.0, 1e-10);
}

TEST_F(qr_test, house_holder) {
	cv::Mat in_ = in->clone();
	QR_dense qr(in_);
	qr.house_holder();
	cv::Mat d = (in_ - qr.Q*qr.R);
	double d_val = cv::norm(d);
	EXPECT_NEAR(d_val, 0.0, 1e-10);
}

TEST_F(qr_test, givens) {
	cv::Mat in_ = in->clone();
	QR_dense qr(in_);
	qr.givens();
	//std::cout << qr.Q << std::endl;
	//std::cout << qr.R << std::endl;
	cv::Mat d = (in_ - qr.Q*qr.R);
	double d_val = cv::norm(d);
	EXPECT_NEAR(d_val, 0.0, 1e-10);
}