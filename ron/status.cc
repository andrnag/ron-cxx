#include "status.hpp"

namespace ron {

const Status Status::OK{};
const Status Status::NOT_IMPLEMENTED{1152921504606846975UL};
const Status Status::NOT_FOUND{428933766657507328UL};
const Status Status::BAD_STATE{201032812431266688UL};
const Status Status::DB_FAIL{237442776878546944UL};
const Status Status::CHAINBREAK{221003099021304468UL};
const Status Status::HASHBREAK{309183850229572864UL};
const Status Status::TREEBREAK{530079928137852160UL};

const Status Status::REPEAT{490440333889372160UL};
const Status Status::REORDER{490436832172703744UL};

const Status Status::TREEGAP{530079933224189952UL};
const Status Status::YARNGAP{615424644247453696UL};

}  // namespace ron
