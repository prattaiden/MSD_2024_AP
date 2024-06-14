import std.stdio;
import std.math;
import std.algorithm;
import std.range;
import std.typecons;
import common;
   
//alias Dim = 2;
alias P2 = Point!2;


void main();

struct quadTree{
    //should call the recursive node constructor
    quadTreeNode* root;

    //  this(P2[] points) {
    //     root = cast(quadTreeNode*) new quadTreeNode(points, boundingBox(points));
    // }

    struct quadTreeNode{
        // alias Point!2 = Point2; 
        // alias AABB!2 = AABB2;
        AABB!2 aabb; 
        Point!2[] points;
        quadTreeNode* NW;
        quadTreeNode* NE;
        quadTreeNode* SW;
        quadTreeNode* SE;
        bool isLeaf = false;

        this(Point!2[] points, AABB!2 aabb){
            this.NW = null;
            this.NE = null;
            this.SW = null;
            this.SE = null;
      

            //threshold is '4' used to determine if we should continuously keep splitting 
            if(points.length < 4){
                this.points = points;
                isLeaf = true; 
            }
            else{
    
            //recursive   
            auto boundary = boundingBox(points);
            this.aabb = boundary;

            auto midX = (boundary.max[0] + boundary.min[0]) / 2;
            auto midY = (boundary.max[1] + boundary.min[1]) / 2;

            auto rightHalfPoints = points.partitionByDimension!0(midX);
            auto leftHalfPoints = points[0 .. $ - rightHalfPoints.length];

            //left side
            auto topLeftPoints = leftHalfPoints.partitionByDimension!1(midY);
            auto bottomLeftPoints = leftHalfPoints[0 .. $ - topLeftPoints.length];

            //right side
            auto topRightPoints = rightHalfPoints.partitionByDimension!1(midY);
            auto bottomRightPoints = rightHalfPoints[0 .. $ - rightHalfPoints.length];

            //creating the bounding boxes of those new quadrants found
            AABB!2 topLeftBox;
            topLeftBox.min = P2([boundary.min[0], midY]);
            topLeftBox.max = P2([midX, boundary.max[1]]);
            
            AABB!2 topRightBox;
            topRightBox.min = P2([midX, midY]);
            topRightBox.max = P2([boundary.max[0], boundary.max[1]]);
            
            AABB!2 bottomLeftBox;
            bottomLeftBox.min = P2([boundary.min[0], boundary.min[1]]);
            bottomLeftBox.max = P2([midX, midY]);
            
            AABB!2 bottomRightBox;
            bottomRightBox.min = P2([midX, boundary.min[1]]);
            bottomRightBox.max = P2([boundary.max[0], midY]);
            //making children for this node, each of those is a new node with a new quarter and points in that array
            //nw is build a new node of out list of nodes in top left corner etc..
            //this calls the constructor for the top left values
            this.NW = cast(quadTreeNode*) new quadTreeNode(topLeftPoints, topLeftBox);
            this.NE = cast(quadTreeNode*) new quadTreeNode(topRightPoints, topRightBox);
            this.SW = cast(quadTreeNode*) new quadTreeNode(bottomLeftPoints, bottomLeftBox);
            this.SE = cast(quadTreeNode*) new quadTreeNode(bottomRightPoints, bottomRightBox);

            }
        }
    }

    bool intersectsPointCircle(P2 p, float r, AABB!2 aabb ){
            bool isIntersecting = false;
            P2 closestPoint = closest(aabb, p);
            writeln("CLOSEST POINT:  " ,closestPoint);
            if(distance(closestPoint , p) < r){
                isIntersecting = true;
            }
        return isIntersecting;
    }



    P2[] rangeQuery(P2 p, float r){
        writeln("executing rangequery function");
        P2[] ret;
        void recurseRangeQuery(quadTreeNode* n){
            writeln("in recursive!");
            if(n.isLeaf){
                //iterate through the points and check if they are within r to p
                foreach(point ; n.points){
                    if(point == p){
                        continue; 
                    }
                    if(distance(p , point) <= r){
                        ret ~= point;
                        writeln("added point:" ,point);
                    }
                }
            }
            else{
            //if not a leaf, recurse children
                if (intersectsPointCircle(p, r, n.NW.aabb)){
                    recurseRangeQuery(n.NW);
                }
                if (intersectsPointCircle(p, r, n.NE.aabb)){
                recurseRangeQuery(n.NE);
                }
                if (intersectsPointCircle(p, r, n.SW.aabb)){
                recurseRangeQuery(n.SW);
                }
                if (intersectsPointCircle(p, r, n.SE.aabb)){
                recurseRangeQuery(n.SE);
                }   
    
            }
        }
        recurseRangeQuery(root);
        return ret;
    }

    //knn
    P2[] KNNQuery(P2 p , size_t k){
        P2[] ret;
        auto pq = makePriorityQueue(p);

        void recurseKNNQuery(quadTreeNode* n){
            if(n.isLeaf){
                foreach (point; n.points) {
                    double dist = (point.data[0] - p.data[0]) * (point.data[0] - p.data[0]) + (point.data[1] - p.data[1]) * (point.data[1] - p.data[1]);
                    if (pq.length < k || dist < distance(pq.front(), p)) {
                        if (pq.length >= k) {
                            pq.removeFront();
                        }
                        pq.insert(point);
                    }
                }
            }else{
            P2 closestPoint = closest(n.aabb, p);
            // Recurse into the child node containing closestPoint
                if (closestPoint[0] <= n.aabb.min[0] + (n.aabb.max[0] - n.aabb.min[0]) / 2) {
                    if (closestPoint[1] <= n.aabb.min[1] + (n.aabb.max[1] - n.aabb.min[1]) / 2) {
                        if (n.SW !is null) recurseKNNQuery(n.SW);
                    } else {
                        if (n.NW !is null) recurseKNNQuery(n.NW);
                    }
                } else {
                    if (closestPoint[1] <= n.aabb.min[1] + (n.aabb.max[1] - n.aabb.min[1]) / 2) {
                        if (n.SE !is null) recurseKNNQuery(n.SE);
                    } else {
                        if (n.NE !is null) recurseKNNQuery(n.NE);
                    }
                }
            }
        }
        recurseKNNQuery(root);

        // Get the k nearest neighbors from the priority queue
        ret = pq.array;

        return ret;
    }
}




//range query unit test
unittest{
    //points array
    P2[] points = [Point!2([.5, .5]), Point!2([1, 1]),
                   Point!2([0.75, 0.4]), Point!2([0.4, 0.74])];

    //quad tree instance using the points array
    AABB!2 testBox = boundingBox(points);
    auto qtTest = quadTree();
    qtTest.root =  new quadTree.quadTreeNode(points, testBox);

    auto rangePoints = qtTest.rangeQuery(P2([1,1]), 0.7);

    writeln("rp length: " , rangePoints.length);
    assert(rangePoints.length == 1);

}


unittest {
    P2[] points = [P2([0, 0]), P2([1, 1]), P2([2, 2]), P2([10, 10])];
    AABB!2 testBox = boundingBox(points);
    auto qTest2 = quadTree(); 
    qTest2.root = new quadTree.quadTreeNode(points, testBox);

    P2 center = P2([1, 1]);
    int k = 3;
    writeln("center: ", center);
    P2[] nearest = qTest2.KNNQuery(center, k);
    writeln("nearest length: ", nearest.length);
    assert(nearest.length == 3, "Should find exactly three nearest neighbors");
    assert(nearest.array.canFind(P2([0, 0])), "Should find P2([0, 0]) as nearest");
    assert(nearest.array.canFind(P2([2, 2])), "Should find P2([2, 2]) as nearest");
    assert(nearest.array.canFind(P2([1, 1])), "Should find P2([1, 1]) as one of nearest");
}

//knn query unit test
// unittest {
//     // Points array
//     P2[] points = [Point!2([.5, .5]), Point!2([1, 1]),
//                    Point!2([0.75, 0.4]), Point!2([0.4, 0.74])];

//     // Quad tree instance using the points array
//     AABB!2 testBox = boundingBox(points);
//     auto qtTest = new quadTree();
//     qtTest.root = new quadTree.quadTreeNode(points, testBox);

//     // Perform KNN query for k=2 at point (1, 1)
//     P2[] knnPoints = qtTest.KNNQuery(P2([1, 1]), 2);

//     // Ensure that the result contains exactly 2 nearest neighbors
//     assert(knnPoints.length == 2);

//     // Ensure that the returned points are the expected nearest neighbors
//     assert(knnPoints[0] == Point!2([1, 1]));  // (1, 1) itself should be closest
//     assert(knnPoints[1] == Point!2([.75, .4]));  // Next closest point

//     // Perform KNN query for k=3 at point (0.6, 0.6)
//     P2[] knnPoints2 = qtTest.KNNQuery(P2([0.6, 0.6]), 3);

//     // Ensure that the result contains exactly 3 nearest neighbors
//     assert(knnPoints2.length == 3);

//     // Ensure that the returned points are the expected nearest neighbors
//     assert(knnPoints2[0] == Point!2([.5, .5]));
//     assert(knnPoints2[1] == Point!2([0.75, 0.4]));
//     assert(knnPoints2[2] == Point!2([0.4, 0.74]));

//     // Perform KNN query for k=1 at point (0, 0)
//     P2[] knnPoints3 = qtTest.KNNQuery(P2([0, 0]), 1);

//     // Ensure that the result contains exactly 1 nearest neighbor
//     assert(knnPoints3.length == 1);

//     // Ensure that the returned point is the expected nearest neighbor
//     assert(knnPoints3[0] == Point!2([.5, .5]));
// }
