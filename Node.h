class Node {
  public:
    Node(void);
    explicit operator bool() const;
    void remove(void);
    void reset(void);

  private:
    bool exists;
};
