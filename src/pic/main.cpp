#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#include <iostream>
#include <experimental/filesystem>
#include <queue>
#include <vector>
#include <utility>

#include "lib/piccompare/cv.h"
#include "lib/piccompare/sift.h"
#include "lib/piccompare/imgfeatures.h"
#include "lib/piccompare/kdtree.h"
#include "lib/piccompare/xform.h"
#include "lib/piccompare/utils.h"

#include "mapper/mapper.h"

// maximum number of keypoint NN candidates to check during BBF search
#define KDTREE_BBF_MAX_NN_CHKS 200

// threshold on squared ratio of distances between NN and 2nd NN
#define NN_SQ_DIST_RATIO_THR 0.5

// minimum number of matches to even start ransac
#define MIN_MATCHES_FOR_RANSAC 4

// Returns number of matches between 2 given feature structs
// We do not pass images here, because then we have to recompute source features every call
static unsigned get_ransac_matches(struct feature* feat1, unsigned n1, struct feature* feat2, unsigned n2) {
    

    struct kd_node* kd_root = kdtree_build(feat2, n2);    

    struct feature** nbrs;
    struct feature** RANnb;

    unsigned matches = 0;
    for(unsigned i = 0; i < n1; ++i) {
        struct feature* feat = feat1 + i;
        unsigned k = kdtree_bbf_knn(kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS);
        if(k == 2 && descr_dist_sq(feat, nbrs[0]) < descr_dist_sq(feat, nbrs[1]) * NN_SQ_DIST_RATIO_THR) {
            feat1[i].fwd_match = nbrs[0];
            ++matches;
        }
        free(nbrs);
    }

    if (matches < MIN_MATCHES_FOR_RANSAC)
        return 0;

    int number=0;
    ransac_xform(feat1, n1, FEATURE_FWD_MATCH, lsq_homog, MIN_MATCHES_FOR_RANSAC, 0.25, homog_xfer_err, 27.0, &RANnb, &number);    

    kdtree_release(kd_root);
    return (unsigned) number;
}

static void usage(char* progname) {
    printf("\nUsage: %s /path/to/image.jpg\n", progname);
    exit(-1);
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        usage(argv[0]);
    if (!hasPics())
        return -1;

    IplImage* query = read_jpeg_file(argv[1]);
    struct feature* queryfeat;
    const unsigned n1 = sift_features(query, &queryfeat);

    auto compare = [](std::pair<std::string, unsigned> lhs, std::pair<std::string, unsigned> rhs) {
                        return lhs.second < rhs.second;
                    };

    std::priority_queue<std::pair<std::string, unsigned>, std::vector<std::pair<std::string, unsigned>>, decltype(compare)> queue(compare); 

    for(const auto& p : std::experimental::filesystem::directory_iterator("pics/")) {
        IplImage* other = read_jpeg_file((char*)p.path().c_str());
        struct feature* otherfeat;
        const unsigned n2 = sift_features(other, &otherfeat);
        queue.push(std::make_pair(p.path(), get_ransac_matches(queryfeat, n1, otherfeat, n2)));
        cvReleaseImage(&other);
        free(otherfeat);
    }
    cvReleaseImage(&query);
    free(queryfeat);

    while(!queue.empty()) {
        std::cout << queue.top().first << '\n';
        queue.pop();
    }

    return 0;
}
