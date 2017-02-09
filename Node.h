class Node {
  public:
    Node(void);
    explicit operator bool() const;
    void remove(void);

  private:
    bool exists;
};
