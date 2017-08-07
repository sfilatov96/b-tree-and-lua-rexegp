package main

import "fmt"

const MIN_DEGREE = 3

var degree int // Параметр дерева

type node struct {
	is_leaf bool    // Является ли узел листом(нет потомков)
	is_root bool    // Является ли узел основанием всего дерева (Устанавливается один раз)
	n       int     // Количество ключей в массиве
	keys    []int   // Массив ключей
	p       []*node // Массив указтелей на другие узлы
}

/* Очищает узел(используется каждый раз при создании узла)*/

func clear_node(root *node) {
	root.keys = make([]int, degree)
	root.p = make([]*node, degree+1)
	for i := 0; i < degree; i++ {
		root.keys[i] = 0
		root.p[i] = nil
	}
	root.n = 0
	root.p[degree] = nil
	root.is_leaf = true
	root.is_root = false
}

func print_b_tree(root *node) {
	fmt.Print("|")
	for i := 0; i != root.n; i++ {
		fmt.Printf("%d ", root.keys[i])
	}
	fmt.Print("|")
	for i := 0; i != root.n+1; i++ {
		if root.p[i] != nil {
			print_b_tree(root.p[i])
		}
	}
}

/* Перепроверяет всё дерево на листы и -не листы  и изменяет параметр root->is_leaf*/
func check_leaf(root *node) {
	for i := 0; i != root.n+1; i++ {
		if root.p[i] != nil {
			root.is_leaf = false
			check_leaf(root.p[i])
		} else {
			root.is_leaf = true
		}
	}
}

/* Добавлеят новое значение в массив ключей */
func add_value_in_array(root *node, value int) int {
	if root.is_leaf == true {
		step := root.n
		for i := 0; i < degree; i++ {
			if root.keys[i] > value {
				step = i
				break
			}
		}
		for j := degree - 1; j > step; j-- {
			root.keys[j] = root.keys[j-1]
		}
		root.keys[step] = value
		root.n = root.n + 1
		return 0
	} else {
		if root.n == degree {
			return 0
		}
		if root.keys[0] > value {
			add_value_in_array(root.p[0], value)
		}
		if root.keys[root.n-1] < value {
			add_value_in_array(root.p[root.n], value)
		}
		for i := 0; i < root.n; i++ {
			if (root.keys[i] < value) && (root.keys[i+1] > value) {
				add_value_in_array(root.p[i+1], value)
			}
		}
	}
	return 0
}

/* Делит массив ключей и массив указателей на левую и правую половины*/
func split(root *node) int {
	var split_pos int
	if degree%2 == 0 {
		split_pos = degree/2 - 1
	} else {
		split_pos = degree / 2
	}
	var ptr *node = nil
	for k := 0; k < root.n+1; k++ {
		ptr = root.p[k]
		if ptr != nil && ptr.n >= degree {
			middle_val := ptr.keys[split_pos]
			var left *node = new(node)
			var right *node = new(node)
			clear_node(left)
			clear_node(right)
			for j := 0; j < degree; j++ {
				if j < split_pos {
					left.keys[j] = ptr.keys[j]
					left.p[j] = ptr.p[j]
					left.n = left.n + 1
				}
				if j > split_pos {
					right.keys[j-split_pos-1] = ptr.keys[j]
					right.p[j-split_pos] = ptr.p[j+1]
					right.n = right.n + 1
				}
			}
			left.p[split_pos] = ptr.p[split_pos]
			right.p[0] = ptr.p[split_pos+1]

			step := root.n
			for i := 0; i < root.n; i++ {
				if root.keys[i] > middle_val {
					step = i
					break
				}
			}
			for j := degree - 1; j >= step; j-- {
				root.keys[j] = root.keys[j-1]
				root.p[j+1] = root.p[j]
			}
			root.p[step] = left
			root.p[step+1] = right
			root.keys[step] = middle_val
			root.n = root.n + 1
		}
	}
	return 0
}

/* Перестраивает корень(основание) дерева если количесво ключей в узле достигло максимума*/
func rebuild_root(root *node) int {
	if root.is_root == true {
		if root.n >= degree {
			var split_pos int
			if degree % 2 == 0 {
				split_pos = degree/2 - 1
			} else {
				split_pos = degree / 2
			}
			var middle_val int
			middle_val = root.keys[split_pos]
			var left *node = new(node)
			var right *node = new(node)
			clear_node(left)
			clear_node(right)
			for j := 0; j < degree; j++ {
				if j < split_pos {
					left.keys[j] = root.keys[j]
					left.p[j] = root.p[j]
					left.n = left.n + 1
				}
				if j > split_pos {
					right.keys[j-split_pos-1] = root.keys[j]
					right.p[j-split_pos ] = root.p[j+1]
					right.n = right.n + 1
				}
			}
			left.p[split_pos] = root.p[split_pos]
			right.p[0] = root.p[split_pos+1]

			if root.p[0] != nil {
				root.is_leaf = false
			}

			root.p[0] = left
			root.p[1] = right
			root.n = 1
			root.keys[0] = middle_val
			root.is_leaf = false
		}
	}
	return 0
}

/* Перестраивает узлы в дереве если узлы переполнены */
func rebuild_b_tree(root *node) int {
	if root != nil {
		for i := 0; i < root.n+1; i++ {
			rebuild_b_tree(root.p[i])
		}
		split(root)
	}
	return 0
}

func insert(root *node, value int) {
	check_leaf(root)
	add_value_in_array(root, value)
	rebuild_b_tree(root)
	rebuild_root(root)
	print_b_tree(root)
}

func get(root *node, value int) int {
	var ptr *node = root
	for ptr != nil {
		for i := 0; i < ptr.n; i++ {
			if ptr.keys[i] == value {
				fmt.Printf("Value %d found in B-tree!\n", ptr.keys[i])
				return 0
			}
		}
		if ptr.is_leaf == true {
			fmt.Printf("Value not found in B-tree!\n")
			return -1
		}
		if ptr.keys[0] > value {
			ptr = ptr.p[0]
		}
		if ptr.keys[ptr.n-1] < value {
			ptr = ptr.p[ptr.n]
		}

		for i := 0; i < ptr.n; i++ {
			if (ptr.keys[i] < value) && (ptr.keys[i+1] > value) {
				if ptr.p[i+1] != nil {
					ptr = ptr.p[i+1]
				}
			}
		}
	}

	return 0
}

func main() {
	fmt.Printf("B-tree model Builder!\n")
	fmt.Printf("a -- Added new value in B-tree\n")
	fmt.Printf("f -- Find value in tree\n")
	fmt.Printf("d -- Display all data from B-tree\n")
	fmt.Printf("q -- Quit from program\n")

	fmt.Printf("Enter B-tree degree: ")
	fmt.Scanf("%d", &degree)
	if degree < MIN_DEGREE {
		fmt.Printf("Bad degree! Degree must be greater than 2!\n")
		return
	}

	root := new(node)
	clear_node(root)
	root.is_root = true
	var choice string
	var value int
	for true {
		fmt.Printf("\nEnter command: ")

		fmt.Scanf("%s", &choice)
		switch choice {
		case "a":
			fmt.Printf("Enter Value: ")
			fmt.Scanf("%d", &value)
			if get(root, value) == 0 {
				fmt.Printf("\nDuplicate error! That value is already exists!\n")
			} else {
				insert(root, value)
				fmt.Printf("\nSuccesful Added!\n")
			}
			break
		case "f":
			fmt.Printf("Enter Value: ")
			fmt.Scanf("%d", &value)
			get(root, value)
			break
		case "d":
			print_b_tree(root)
			break
		case "q":
			fmt.Printf("Bye!\n")
			return
		default:
			fmt.Printf("Undefined Command!\n")
			break
		}
	}
}