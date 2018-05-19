//
// Created by yj374 on 5/14/18.
//

#ifndef ECE4960_SP18_SPARSE_QR_H
#define ECE4960_SP18_SPARSE_QR_H

#include <sparse/matrix/sparse_matrix.h>
#include <opencv2/opencv.hpp>

// QR Decomposition
// M = QR, M = [m x n], m >= n

namespace sparse {

  // Gram-Schmidt Process
class QR {
	// NOT IMPLEMENTED
  QR(sparse_matrix<double> sp);
};
}; // namespace sparse

class QR_dense {
public:
  cv::Mat A;
  cv::Mat Q;
  cv::Mat R;
  int n, m;

  QR_dense(cv::Mat in) {
    n = in.size().width;
    m = in.size().height;
    A = in.clone();	
  }

  void gram_schmidt() {
    Q = cv::Mat(A.size().height, A.size().width, CV_64F); Q.setTo(0);
    R = cv::Mat(A.size().width, A.size().width, CV_64F); R.setTo(0);
    for( int j=0; j<n; j++) {
      cv::Mat uj = A.col(j);
      for( int i=0; i<j; i++) {
        cv::Mat qi = Q.col(i);
        double rij = qi.dot(A.col(j));
        R.at<double>(i, j) = rij;
        uj = uj - rij*qi;
      }
      double rjj = cv::norm(uj);
      R.at<double>(j, j) = rjj;
      for(int i=0; i<m; i++) {
        Q.at<double>(i, j) = uj.at<double>(i) / rjj;
      }
    }
  }

  void house_holder() {
	  Q = cv::Mat::eye(m, m, CV_64F);
	  R = A.clone();
	  for (int j = 0; j < n - 1; j++) {
		  cv::Mat subR = R(cv::Rect(j,j,n-j,m-j));
		  cv::Mat x = subR.col(0);
		  cv::Mat e = cv::Mat::zeros(m - j, 1, CV_64F);
		  e.at<double>(0) = x.at<double>(0) > 0.0 ? 1.0 : -1.0;
		  double nx = cv::norm(x);
		  cv::Mat v = x + nx * e;
		  v = v / cv::norm(v);
		  cv::Mat H_hat = cv::Mat::eye(m - j, m - j, CV_64F) - 2 * v*v.t();
		  cv::Mat H = cv::Mat::eye(m, m, CV_64F);
		  H_hat.copyTo(H(cv::Rect(j, j, m - j, m - j)));
		  R = H*R;
		  Q = Q*H;
	  }
  }

  cv::Mat givens_rotate(int m, int n, double c, double s, int j, int i) {
		cv::Mat q_ = cv::Mat::eye(m, m, CV_64F);
		q_.at<double>(i, i) = c;
		q_.at<double>(j, j) = c;
		q_.at<double>(j, i) = -s;
		q_.at<double>(i, j) = s;
		return q_;
  }  

  void givens() {
	  R = A.clone();
	  Q = cv::Mat::eye(m, m, CV_64F);
	  for (int j = 0; j < m; j++) {
		  for (int i = j + 1; i < m; i++) {
			  double xj = R.at<double>(i, j);
			  double xi = R.at<double>(j, j);
			  double c = xi / std::sqrt(xi * xi + xj * xj);
			  double s = -xj / std::sqrt(xi * xi + xj * xj);
			  cv::Mat q_ = givens_rotate(m, n, c, s, j, i);
			  R = q_*R;
			  Q = Q*q_.t();
		  }
	  }
  }
};

#endif //ECE4960_SP18_SPARSE_QR_H
