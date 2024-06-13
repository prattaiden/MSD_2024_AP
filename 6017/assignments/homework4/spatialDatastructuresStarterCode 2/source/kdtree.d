
import common;
import std.algorithm;
import std.math;
import std.container.binaryheap;
import std.range;

//ben questions
//utilize methods from common to help build the tree, i used the medianbydimension
//help implementing the range query 

//Creating the node struct for the KD tree
//needs a point, the data 
//needs the left and right children
//nodes are classes so pointers are on the heap 
class KDNode(size_t Dim){
   // c
    KDNode* leftChild;
    KDNode* rightChild;

    //constructor for KDNode
    this(Point!Dim point){
        this.point = point;
        this.leftChild = null;
        this.rightChild = null;
    }
}


//helper function used to calculate the squared distance between two points
float squaredDistance(Dim)(Point!Dim p1, Point!Dim p2){
    float sum = 0;
    foreach(i; 0 .. Dim){
        sum+= pow(p1[i] - p2[i],2);
    }
    return sum;
}

//creating the KD Tree strcut
struct KDTree(size_t Dim){
    KDNode!Dim* root;

    //constructor for the KDTree
    this(Point!Dim[] points){
        root = buildKDTree(points, 0);
    }


    //method to build the KD Tree recursively 
    KDNode!Dim* buildKDTree(Point!Dim[] points, size_t depth){
        if(points.empty) return null;
        
        //determining the splitting axis based on curreny depth of the tree
        size_t axis = depth % Dim;
        //median helper function which puts points to the left of the median with smaller coordinates and right with larger coordinates
        points.medianByDimension!(axis);
        size_t median = points.length/2;

        auto node = new KDNode!Dim(points[median]);
        //recursive 
        node.leftChild = buildKDTree(points[0 .. median], depth + 1);
        node.rightChild = buildKDTree(points[median + 1 .. $], depth + 1);

        return node;
    }


    //now to implement the query methods
    //range query
    //find and return all points within a specified radius `r` from a target point `p`
    Point!Dim[] rangeQuery(Point!Dim p, float r){
        Point!Dim[] resultArray;
        rangeQueryRecursive(root, p, r, result);
        return result;
    }
    //helper recursive function to do the searching 
    void rangeQueryRecursive(KDNode!Dim* node, Point!Dim p, float r, Point!Dim[] result){
        if(node is null) return; //reached a leaf node

        if(squaredDistance(node.point,p) <= r * r){
            result ~= node.point; //append that point
        }

    // Check if the splitting plane intersects with the circle defined by p and r
        // if (abs(node.point[axis] - p[axis]) <= r) {
        //     // Recurse on both children since they may contain points within the range
        //     rangeQueryRecursive(node.leftChild, p, r, result);
        //     rangeQueryRecursive(node.rightChild, p, r, result);
        // } else if (node.point[axis] < p[axis]) {
        //     rangeQueryRecursive(node.rightChild, p, r, result);
        // } else {
        //     rangeQueryRecursive(node.leftChild, p, r, result);
        // }

    }

}


//unit test for rangequery in the KDTree
// unittest{
//        auto points = [Point!2([.5, .5]), Point!2([1, 1]),
//                    Point!2([0.75, 0.4]), Point!2([0.4, 0.74])];

//     /// making a KDTree with these points
//     auto kdtree_test = KDTree!(2)(points);

//     writelin("kdtree range query test");
//     auto rangeResult = kdtree.rangeQuery(Point!2([1,1]), .7);
//     writeln("points within range: ");
//     foreach(p; rangeResult){
//         writeln(p);
//     }
// }